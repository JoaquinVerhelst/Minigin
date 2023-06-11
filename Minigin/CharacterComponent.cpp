#include "CharacterComponent.h"
#include "Command.h"
#include "InputManager.h"
#include "Time.h"

#include "Renderer.h"
#include "Minigin.h"
#include "World.h"

#include <iostream>
#include "SimpleRenderComponent.h"
#include "HealthComponent.h"
#include "SoundServiceLocator.h"

namespace dae
{




	CharacterComponent::CharacterComponent(GameObject* owner, float walkSpeed, bool isControlledByPlayer)
		: dae::Component(owner)
		, m_WalkSpeed{ walkSpeed }
		, m_CellSize{ 0 }
		, m_CurrentState{ new IdleState()}
		, m_InputID{0}
		, m_Direction{0}
		, m_CurrentCell{ nullptr }
		, m_ControlledByPlayer{ isControlledByPlayer }
		, m_IsDead{ false }
	{

		if (isControlledByPlayer)
		{
			auto& input = InputManager::GetInstance();
			m_InputID = input.AssignController(GetOwner());
		}
		else
		{
			m_InputID = -1;
		}

		m_CellSize = World::GetInstance().GetCellSize();;
	}

	CharacterComponent::~CharacterComponent()
	{
		delete m_CurrentState;
		m_CurrentState = nullptr;
	}

	void CharacterComponent::HandleInput(Command::InputType inputType, PlayerStateType newStateType)
	{

		CharacterState* newState = m_CurrentState->HandleInput(inputType, newStateType);

		if (newState != nullptr)
		{
			if (newStateType != m_CurrentState->GetType())
			{
				m_CurrentState->UpdateSprite(GetOwner(), GetDirection());
			}

			SetState(newState);
		}
	}


	void CharacterComponent::Update()
	{
		if (!m_CurrentState->CheckCollision(GetOwner(), this))
		{
			m_CurrentState->Update(GetOwner(), this);
		}

		CheckBorders();

	}

	void CharacterComponent::Render() const
	{
		// Renders the broken cells by player

		glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();

		SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), Renderer::GetInstance().GetCanvas());
		SDL_Rect rect = SDL_Rect{ static_cast<int>(currentPos.x) , static_cast<int>(currentPos.y), m_CellSize.x ,m_CellSize.y };
		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 0, 255);
		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);

		SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), nullptr);
	}

	void CharacterComponent::Init()
	{
		SetState(new IdleState());
	}

	bool CharacterComponent::CheckBorders()
	{
		auto pos = GetPosition();

		if (pos.y <= 2 * m_CellSize.y)
		{
			GetOwner()->SetPosition(pos.x, pos.y + 0.1f);
			return false;
		}
		if (pos.y + m_CellSize.y >= 720)
		{
			GetOwner()->SetPosition(pos.x, pos.y - 0.1f);
			return false;
		}
		if (pos.x <= 0)
		{
			GetOwner()->SetPosition(pos.x + 0.1f, pos.y);
			return false;
		}
		if (pos.x + m_CellSize.y >= 1080)
		{
			GetOwner()->SetPosition(pos.x - 0.1f, pos.y);
			return false;
		}

		return true;
	}

	void CharacterComponent::SetState(CharacterState* newState)
	{
		if (m_CurrentState != nullptr)
		{
			delete m_CurrentState;
			m_CurrentState = nullptr;
		}

		m_CurrentState = newState;
		m_CurrentState->UpdateSprite(GetOwner(), m_Direction);
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



	void CharacterComponent::GetDamaged()
	{
	}


	void CharacterComponent::Respawn()
	{
	}

	void CharacterComponent::UpdateDeath()
	{
	}




	glm::vec3 CharacterComponent::GetPosition()
	{
		return GetOwner()->GetPosition().GetPosition();
	}

	void CharacterComponent::SetDirection(Direction direction)
	{
		if (m_CurrentState->GetType() != PlayerStateType::Dead && m_Direction != direction)
		{
			m_Direction = direction;
			m_CurrentState->UpdateSprite(GetOwner(), direction);
		}
	}

	Direction CharacterComponent::GetDirection()
	{
		return m_Direction;
	}

	glm::ivec2 CharacterComponent::GetCellSize()
	{
		return m_CellSize;
	}




}