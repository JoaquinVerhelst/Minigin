#include "DiggerComponent.h"
#include "Time.h"
#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"

namespace dae
{

	DiggerComponent::DiggerComponent(GameObject* owner, float walkSpeed)
		: dae::Component(owner)
		, m_WalkSpeed{ walkSpeed }
	{
		auto& input = InputManager::GetInstance();
		m_InputID = input.AssignController(GetOwner());
	}


	void DiggerComponent::Update()
	{

	}

	void DiggerComponent::Render() const
	{
	}

	void DiggerComponent::Init()
	{
	}



	void DiggerComponent::Walk(int direction)
	{


		glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();

		auto& time = Time::GetInstance();

		switch (direction)
		{
		case 0:
			GetOwner()->SetPosition(currentPos.x, currentPos.y - m_WalkSpeed * time.GetDeltaTime());
			break;
		case 1:
			GetOwner()->SetPosition(currentPos.x, currentPos.y + m_WalkSpeed * time.GetDeltaTime());
			break;
		case 2:
			GetOwner()->SetPosition(currentPos.x + m_WalkSpeed * time.GetDeltaTime(), currentPos.y);
			break;
		case 3:
			GetOwner()->SetPosition(currentPos.x - m_WalkSpeed * time.GetDeltaTime(), currentPos.y);
			break;
		}
		
	}

}