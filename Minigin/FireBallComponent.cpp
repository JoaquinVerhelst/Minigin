#include "FireBallComponent.h"
#include "SimpleRenderComponent.h"
#include "Time.h"
#include "CharacterComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "NobbinManager.h"
#include "NobbinComponent.h"
#include "ScoreComponent.h"
#include "SoundServiceLocator.h"


namespace dae
{
	dae::FireBallComponent::FireBallComponent(GameObject* owner, GameObject* digger, glm::vec2 position, Direction direction)
		: Component(owner)
		, m_Position{ }
		, m_Direction{ direction }
		, m_Speed{ 100.f }
		, m_Size{}
		, m_DiggerOwner{ digger }
	{

		m_Size = GetOwner()->GetComponent<SimpleRenderComponent>().GetTexture()->GetSize();

		m_Position = glm::vec2(position.x - m_Size.x / 2, position.y - m_Size.y / 2);

		GetOwner()->SetPosition(m_Position.x, m_Position.y);
	}

	FireBallComponent::~FireBallComponent()
	{
		m_DiggerOwner = nullptr;
	}

	void dae::FireBallComponent::Update()
	{
		CalculateMovement();
		CheckCollision();

		if (World::GetInstance().IsOverlappingWithWorld(m_Position, m_Size))
		{
			GetOwner()->~GameObject();
		}
	}

	void dae::FireBallComponent::Init()
	{
		LevelInfo* levelInfo = SceneManager::GetInstance().GetCurrentSceneLevelInfo();
		m_Nobbins = levelInfo->nobbinManager->GetComponent<NobbinManager>().GetNobbins();
	}

	void dae::FireBallComponent::CalculateMovement()
	{
		auto currentPos = GetOwner()->GetPosition().GetPosition();
		float deltaTime = Time::GetInstance().GetDeltaTime();

		switch (m_Direction)
		{
		case Direction::Down:
			m_Position = glm::vec2(currentPos.x, currentPos.y - m_Speed * deltaTime);
			break;

		case Direction::Up:
			m_Position = glm::vec2(currentPos.x, currentPos.y + m_Speed * deltaTime);
			break;

		case Direction::Left:
			m_Position = glm::vec2(currentPos.x + m_Speed * deltaTime, currentPos.y);
			break;

		case Direction::Right:
			m_Position = glm::vec2(currentPos.x - m_Speed * deltaTime, currentPos.y);
			break;
		}

		GetOwner()->SetPosition(m_Position.x, m_Position.y);
	}

	void dae::FireBallComponent::CheckCollision()
	{
		for (size_t n = 0; n < m_Nobbins.size(); ++n)
		{
			const glm::vec2& nobbin = m_Nobbins[n]->GetPosition().GetPosition();


			if (m_Position.x + m_Size.x > nobbin.x && m_Position.x < nobbin.x + m_Size.x &&
				m_Position.y + m_Size.y > nobbin.y && m_Position.y < nobbin.y + m_Size.y)
			{
				m_DiggerOwner->GetComponent<ScoreComponent>().AddEmeraldScore(250);
				
				SceneManager::GetInstance().GetCurrentSceneLevelInfo()->nobbinManager->GetComponent<NobbinManager>().DestroyNobbin(n);

				m_Nobbins.erase(m_Nobbins.begin() + n);

				GetOwner()->~GameObject();

			}
		}
	}
}
