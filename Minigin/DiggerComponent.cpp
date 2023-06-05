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

namespace dae
{

	DiggerComponent::DiggerComponent(GameObject* owner, float walkSpeed)
		: dae::CharacterComponent(owner, walkSpeed, true)
		, m_DeathCounter{0}
		, m_DeathTime{2.f}
		, m_IsDamaged{ false }
	{
		SetState(new IdleState);
	}


	void DiggerComponent::Update()
	{


		CharacterComponent::Update();


		World::GetInstance().BreakWorld(GetOwner(), { m_CellSize.x - 1.f,m_CellSize.y - 1.f });


	}


	void DiggerComponent::Init()
	{
		CharacterComponent::Init();
	}

	void DiggerComponent::Render() const
	{
		CharacterComponent::Render();
	}



	void DiggerComponent::GetDamaged()
	{
		if (!m_IsDamaged)
		{
			HealthComponent& healthComp = GetOwner()->GetComponent<HealthComponent>();

			if (healthComp.GetHealth() == 0)
			{
				healthComp.SetHealth(0);
			}
			else
			{
				healthComp.SetHealth(healthComp.GetHealth() - 1);
			}

			SoundServiceLocator::GetSoundSystem().Play(0, 1.0f);

			m_IsDamaged = true;
		}

	}

	void DiggerComponent::Shoot()
	{
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

}