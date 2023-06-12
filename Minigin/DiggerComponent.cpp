#include "DiggerComponent.h"
#include "Time.h"
#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"
#include "SoundServiceLocator.h"
#include "HealthComponent.h"
#include "SimpleRenderComponent.h"
#include "World.h"
#include "SceneManager.h"
#include "Scene.h"
#include "FireBallComponent.h"

namespace dae
{

	DiggerComponent::DiggerComponent(GameObject* owner, float walkSpeed)
		: dae::CharacterComponent(owner, walkSpeed, true)
		, m_DeathCounter{0}
		, m_DeathTime{2.f}
		, m_IsDamaged{ false }
		, m_FireBall{ nullptr }
		, m_ShootCount{0.f}
		, m_ShootDelay{9.f}
		, m_CanShoot{ true }
		, m_Sprites{nullptr}
	{
		SetState(new IdleState);
	}

	DiggerComponent::~DiggerComponent()
	{
		delete m_Sprites;
	}


	void DiggerComponent::Update()
	{


		CharacterComponent::Update();


		World::GetInstance().BreakWorld(GetOwner(), { m_CellSize.x ,m_CellSize.y });

		if (m_FireBall)
		{
			m_FireBall->Update();
		}

		if (!m_CanShoot)
		{
			m_ShootCount += Time::GetInstance().GetDeltaTime();
			
			if (m_ShootCount >= m_ShootDelay)
			{
				m_CanShoot = true;
				m_ShootCount = 0;
				GetOwner()->GetComponent<SimpleRenderComponent>().SetTexture(m_Sprites->playerSprite);
			}
		}
	}


	void DiggerComponent::Init()
	{
		CharacterComponent::Init();
	}

	void DiggerComponent::Render() const
	{
		CharacterComponent::Render();

		if (m_FireBall)
		{
			m_FireBall->Render();
		}
	}



	void DiggerComponent::GetDamaged()
	{


		if (!m_IsDamaged)
		{
			SetState(new DamagedState());
			HealthComponent& healthComp = GetOwner()->GetComponent<HealthComponent>();
			healthComp.SetHealth(healthComp.GetHealth() - 1);
			SoundServiceLocator::GetSoundSystem().Play(0, 1.0f);

			m_IsDamaged = true;
		}

	}

	void DiggerComponent::UseSpecialty()
	{
		if (m_CanShoot)
		{
			SpawnFireBall();
			GetOwner()->GetComponent<SimpleRenderComponent>().SetTexture(m_Sprites->reloadingSprite);
			m_CanShoot = false;
		}
	}

	void DiggerComponent::Respawn()
	{
		GetOwner()->GetComponent<SimpleRenderComponent>().SetTexture(m_Sprites->playerSprite);
		SetState(new IdleState());
		m_DeathCounter = 0;

		auto grid = World::GetInstance().GetWorldGrid();

		auto max = grid.size();

		bool finish = true;
		int index = 0;

		do
		{
			index = rand() % max;

			if (!grid[index]->isCellBroken || !grid[index]->hasTreasure)
			{
				finish = false;
			}


		} while (finish);




		World::GetInstance().PlaceGameObject(GetOwner(), index);
		m_IsDamaged = 0;
	}

	void DiggerComponent::UpdateDeath()
	{
		m_DeathCounter += Time::GetInstance().GetDeltaTime();

		if (m_DeathCounter >= m_DeathTime)
		{
			Respawn();
		}
	}

	void DiggerComponent::SetIsDead()
	{
		SetState(new DeadState());
	}

	void DiggerComponent::SpawnFireBall()
	{

		auto pos = GetOwner()->GetPosition().GetPosition();
	
		Direction direction;


		switch (m_Direction)
		{
		case dae::Direction::Up:
			direction = Direction::Down;
			break;
		case dae::Direction::Down:
			direction = Direction::Up;
			break;
		case dae::Direction::Left:
			direction = Direction::Right;
			break;
		case dae::Direction::Right:
			direction = Direction::Left;
			break;
		}



		auto fireball = std::make_shared<dae::GameObject>();
		fireball->AddComponent<dae::SimpleRenderComponent>(m_Sprites->fireBallSprite);
		fireball->AddComponent<FireBallComponent>(GetOwner(), glm::vec2(pos.x + m_CellSize.x / 2, pos.y + m_CellSize.y / 2), direction);



		if (m_FireBall)
		{
			m_FireBall = nullptr;
		}

		m_FireBall = fireball;


	}


}