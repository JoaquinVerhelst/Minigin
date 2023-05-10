#pragma once
#include "CharacterComponent.h"
//#include "InputManager.h"
#include "Command.h"

namespace dae
{



	class State
	{
	public:


		State() = default;
		virtual ~State() = default;
		virtual bool HandleInput(CharacterComponent* character, Command::InputType inputType, StateType newStateType) = 0;
		//virtual void Update(CharacterComponent*) {};

		virtual StateType GetType() { return m_Type; }

	protected:
		StateType m_Type{ StateType::Default };

	};


	class HorizontalWalkState : public State
	{
	public:
		HorizontalWalkState()
		{
			m_Type = StateType::HorizontalWalk;
		}

		~HorizontalWalkState() = default;

		bool HandleInput(CharacterComponent* character, Command::InputType inputType, StateType newStateType) override;

	};

	class VerticalWalkState : public State
	{
	public:
		VerticalWalkState()
		{
			m_Type = StateType::VerticalWalk;
		}

		~VerticalWalkState() = default;

		bool HandleInput(CharacterComponent* character, Command::InputType inputType, StateType newStateType) override;

	};




	class IdleState : public State
	{
	public:

		IdleState()
		{
			m_Type = StateType::Idle;
		}

		~IdleState() = default;

		bool HandleInput(CharacterComponent* character, Command::InputType inputType, StateType newStateType) override;
	};


}
