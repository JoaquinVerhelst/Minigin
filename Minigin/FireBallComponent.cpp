#include "FireBallComponent.h"
#include "SimpleRenderComponent.h"
#include "Time.h"

namespace dae
{
	dae::FireBallComponent::FireBallComponent(GameObject* owner, glm::vec2 position, Direction direction)
		: Component(owner)
		, m_Position{ }
		, m_Direction{ direction }
		, m_Speed{ 100.f }
	{

		auto size = GetOwner()->GetComponent<SimpleRenderComponent>().GetTexture()->GetSize();


		m_Position = glm::vec2(position.x - size.x / 2, position.y - size.y / 2);

		GetOwner()->SetPosition(m_Position.x, m_Position.y);
	}

	void dae::FireBallComponent::Update()
	{
		CalculateMovement();
	}

	void dae::FireBallComponent::Init()
	{
	}

	void dae::FireBallComponent::CalculateMovement()
	{
		auto currentPos = GetOwner()->GetPosition().GetPosition();
		float deltaTime = Time::GetInstance().GetDeltaTime();

		switch (m_Direction)
		{
		case Direction::Down:
			GetOwner()->SetPosition(currentPos.x, currentPos.y - m_Speed * deltaTime);

			break;
		case Direction::Up:
			GetOwner()->SetPosition(currentPos.x, currentPos.y + m_Speed * deltaTime);
			break;
		case Direction::Right:
			GetOwner()->SetPosition(currentPos.x + m_Speed * deltaTime, currentPos.y);
			break;
		case Direction::Left:
			GetOwner()->SetPosition(currentPos.x - m_Speed * deltaTime, currentPos.y);
			break;
		}


	}

	void dae::FireBallComponent::CheckCollision()
	{
	}
}
