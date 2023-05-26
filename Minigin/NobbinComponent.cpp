#include "NobbinComponent.h"

#include "Time.h"
#include "World.h"
#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"
#include "PlayerState.h"

namespace dae
{

	NobbinComponent::NobbinComponent(GameObject* owner, float walkSpeed)
		: dae::CharacterComponent(owner, walkSpeed, false)
	{

	}

	NobbinComponent::~NobbinComponent()
	{
	}

	//void NobbinComponent::HorizontalWalk(int direction)
	//{
	//	//if (!m_CurrentState->HandleInput(this, inputType, newStateType))
	//	//	return;

	//	glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();


	//	auto vec = CalculateWalk(direction, m_CellSize.y, currentPos.y, currentPos.x);


	//	GetOwner()->SetPosition(vec.y, vec.x);
	//}

	void NobbinComponent::CalculateMovement()
	{

		glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();

		if (!World::GetInstance().IsOverlappingWithWorld(currentPos, glm::vec2(50, 50)))
		{

			//HorizontalWalk(0, Command::InputType::Pressed, StateType::HorizontalWalk);
		}


	}


	void NobbinComponent::Update()
	{
		//CharacterComponent::Update();


		CalculateMovement();
	}


	void NobbinComponent::Init()
	{
		//m_CurrentState = new IdleState;
	}

	void NobbinComponent::Render() const
	{

		//CharacterComponent::Render();


	}



}