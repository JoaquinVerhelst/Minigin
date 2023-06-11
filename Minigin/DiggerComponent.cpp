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
	{
		SetState(new IdleState);
	}


	void DiggerComponent::Update()
	{


		CharacterComponent::Update();


		World::GetInstance().BreakWorld(GetOwner(), { m_CellSize.x - 1.f,m_CellSize.y - 1.f });

		if (m_FireBall)
		{
			m_FireBall->Update();
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

		SpawnFireBall();

	}

	void DiggerComponent::Respawn()
	{
		GetOwner()->GetComponent<SimpleRenderComponent>().SetTexture("../Data/Sprites/Player.png");
		SetState(new IdleState());
		m_DeathCounter = 0;
		World::GetInstance().PlaceGameObject(GetOwner(), SceneManager::GetInstance().GetCurrentSceneLevelInfo()->player1PosIndex);
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
	
		FireBallComponent::Direction direction;

		if (GetState()->GetType() == PlayerStateType::VerticalWalk)
		{
			if (m_Direction == 0)
			{
				direction = FireBallComponent::Direction::Up;
			}
			else
			{
				direction = FireBallComponent::Direction::Down;
			}
		}
		else if (GetState()->GetType() == PlayerStateType::HorizontalWalk)
		{
			if (m_Direction == 0)
			{
				direction = FireBallComponent::Direction::Right;
			}
			else
			{
				direction = FireBallComponent::Direction::Left;
			}
		}






		auto fireball = std::make_shared<dae::GameObject>();
		fireball->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/FireBall.png");
		fireball->AddComponent<FireBallComponent>(glm::vec2(pos.x + m_CellSize.x / 2, pos.y + m_CellSize.y / 2), direction);



		if (m_FireBall)
		{
			m_FireBall = nullptr;
		}

		m_FireBall = fireball;


	}


}