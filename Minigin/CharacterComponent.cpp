#include "CharacterComponent.h"
#include "Command.h"
#include "InputManager.h"
#include "Time.h"
#include "State.h"
#include "Renderer.h"
#include "Minigin.h"
#include "World.h"

#include <iostream>
#include "SimpleRenderComponent.h"

namespace dae
{




	CharacterComponent::CharacterComponent(GameObject* owner, float walkSpeed, bool isControlledByPlayer)
		: dae::Component(owner)
		, m_WalkSpeed{ walkSpeed }
		, m_CellSize{ 0 }
		, m_CurrentState{ new IdleState }
		, m_InputID{0}
	{
		auto& input = InputManager::GetInstance();


		if (isControlledByPlayer)
		{
			m_InputID = input.AssignController(GetOwner());
		}
		else
		{
			m_InputID = -1;
		}

		glm::vec2 cellSize = World::GetInstance().GetCellSize();

		m_CellSize = cellSize;

	}

	CharacterComponent::~CharacterComponent()
	{
		delete m_CurrentState;
		m_CurrentState = nullptr;
	}

	void CharacterComponent::Update()
	{
		//m_CurrentState->Update(this);

		auto pos = GetOwner()->GetPosition().GetPosition();


		//int currentCellX = pos.x / m_GridCellSize;
		//int currentCellY = pos.y / m_GridCellSize;


		//GetOwner()->GetComponent<SimpleRenderComponent>().

		//std::cout << 


		World::GetInstance().BreakWorld(pos, {30,30});

	}

	void CharacterComponent::Render() const
	{

		glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();

		SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), Renderer::GetInstance().GetCanvas() );
		SDL_Rect rect = SDL_Rect{ static_cast<int>(currentPos.x) , static_cast<int>(currentPos.y), 50 ,50 };
		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 0, 255);
		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	}


	void CharacterComponent::Init()
	{




	}



	void CharacterComponent::Walk(int , int , StateType )
	{



		//if (!m_CurrentState->HandleInput(this, newStateType, inputType))
		//	return;


		//if (direction ==1 || direction == 0)
		//{
		//	WalkVertical(direction);

		//}
		//else if (direction == 2 || direction == 3)
		//{
		//	WalkHorizontal(direction);
		//}


		//switch (direction)
		//{
		//case 0:
		//case 1:

		//	break;
		//case 2:
		//case 3:

		//	break;
		//}

	}

	void CharacterComponent::SetState(State* newState)
	{
		if (m_CurrentState != nullptr)
		{
			delete m_CurrentState;
			m_CurrentState = nullptr;
		}

		m_CurrentState = newState;
	}

	glm::vec2 CharacterComponent::CalculateWalk(int direction, float cellSize, float x, float y)
	{
		float deltaTime = Time::GetInstance().GetDeltaTime();

		int offset = static_cast<int>(x) % static_cast<int>(cellSize);


		if (offset == 0)
		{
			y += (direction == 0 ? m_WalkSpeed * deltaTime : -m_WalkSpeed * deltaTime);
		}
		else
		{
			x += (offset >= cellSize / 2.f ? m_WalkSpeed * deltaTime : -m_WalkSpeed * deltaTime);
		}

		return glm::vec2(x, y);
	}


	void CharacterComponent::HorizontalWalk(int direction, Command::InputType inputType, StateType newStateType)
	{
		if (!m_CurrentState->HandleInput(this, inputType, newStateType))
			return;



		glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();


		auto vec = CalculateWalk(direction, m_CellSize.y, currentPos.y, currentPos.x);


		GetOwner()->SetPosition(vec.y, vec.x);
	}

	void CharacterComponent::VerticalWalk(int direction, Command::InputType inputType, StateType newStateType)
	{
		if (!m_CurrentState->HandleInput(this, inputType, newStateType))
			return;


		glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();


		auto vec = CalculateWalk(direction, m_CellSize.x, currentPos.x, currentPos.y);


		GetOwner()->SetPosition(vec.x, vec.y);

	}




}