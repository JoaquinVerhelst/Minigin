#pragma once

//#include "InputManager.h"
#include "Command.h"

namespace dae
{

	class CharacterComponent;

	enum class PlayerStateType
	{
		Idle,
		HorizontalWalk,
		VerticalWalk

	};

	class PlayerState
	{
	public:


		PlayerState() = default;
		virtual ~PlayerState() = default;
		virtual PlayerState* HandleInput(Command::InputType inputType, PlayerStateType newStateType) = 0;
		virtual void Update(GameObject*, CharacterComponent*) {};

		virtual PlayerStateType GetType() { return m_Type; }

	protected:
		PlayerStateType m_Type{ PlayerStateType::Idle };

	};


	class HorizontalWalkState : public PlayerState
	{
	public:
		HorizontalWalkState()
		{
			m_Type = PlayerStateType::HorizontalWalk;
		}

		~HorizontalWalkState() = default;
		PlayerState* HandleInput( Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
	};

	class VerticalWalkState : public PlayerState
	{
	public:
		VerticalWalkState()
		{
			m_Type = PlayerStateType::VerticalWalk;
		}

		~VerticalWalkState() = default;
		PlayerState* HandleInput( Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
	};




	class IdleState : public PlayerState
	{
	public:

		IdleState()
		{
			m_Type = PlayerStateType::Idle;
		}

		~IdleState() = default;

		PlayerState* HandleInput( Command::InputType inputType, PlayerStateType newStateType) override;
	};


}
