#include "CharacterComponent.h"
#include "Command.h"
#include "InputManager.h"
#include "Time.h"

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
		, m_CurrentState{ new IdleState()}
		, m_InputID{0}
		,m_Direction{0}
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

		glm::ivec2 cellSize = World::GetInstance().GetCellSize();

		m_CellSize = cellSize;




	}

	CharacterComponent::~CharacterComponent()
	{
		delete m_CurrentState;
		m_CurrentState = nullptr;
	}

	void CharacterComponent::HandleInput(Command::InputType inputType, PlayerStateType newStateType)
	{
		PlayerState* newState = m_CurrentState->HandleInput( inputType, newStateType);
		if (newState != nullptr)
		{
			SetState(newState);
		}
	}


	void CharacterComponent::Update()
	{
		m_CurrentState->Update(GetOwner(), this);

		World::GetInstance().BreakWorld(GetOwner(), {30,30});





		glm::ivec3 pos = GetOwner()->GetPosition().GetPosition();
		SDL_Rect rect = SDL_Rect{ pos.x, pos.y, 50, 50 };
		
	}

	void CharacterComponent::Render() const
	{
		glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();

		SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), Renderer::GetInstance().GetCanvas());
		SDL_Rect rect = SDL_Rect{ static_cast<int>(currentPos.x) , static_cast<int>(currentPos.y), 50 ,50 };
		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 0, 255);
		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);

		SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), nullptr);
	}

	void CharacterComponent::Init()
	{
		SetState(new IdleState());

	}





	void CharacterComponent::SetState(PlayerState* newState)
	{
		if (m_CurrentState != nullptr)
		{
			delete m_CurrentState;
			m_CurrentState = nullptr;
		}

		m_CurrentState = newState;
	}

	glm::vec2 CharacterComponent::CalculateWalk(int direction, int cellSize, float x, float y)
	{
		float deltaTime = Time::GetInstance().GetDeltaTime();

		double offset = std::round(static_cast<int>(x) % static_cast<int>(cellSize));

		if (offset == 0)
		{
			y += (direction == 0 ? m_WalkSpeed * deltaTime : -m_WalkSpeed * deltaTime);
			x = static_cast<float>(std::round(static_cast<int>(x)));
		}
		else
		{
			x += (offset >= cellSize / 2.f ? m_WalkSpeed * deltaTime : -m_WalkSpeed * deltaTime);
			y = static_cast<float>(std::round(static_cast<int>(y)));
		}

		return glm::vec2(x, y);
	}




	glm::vec3 CharacterComponent::GetPosition()
	{
		return GetOwner()->GetPosition().GetPosition();
	}



	void CharacterComponent::SetDirection(int direction)
	{
		m_Direction = direction;
	}

	int CharacterComponent::GetDirection()
	{
		return m_Direction;
	}

	glm::ivec2 CharacterComponent::GetCellSize()
	{
		return m_CellSize;
	}




}