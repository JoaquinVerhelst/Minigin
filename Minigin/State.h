#pragma once
#include "CharacterComponent.h"
//#include "InputManager.h"

namespace dae
{



	class State
	{
	public:


		State() = default;
		virtual ~State() = default;
		virtual bool HandleInput(CharacterComponent* character, StateType newStateType, int inputType) = 0;
		//virtual void Update(CharacterComponent*) {};

		virtual StateType GetType() { return m_Type; }

	protected:
		StateType m_Type{ StateType::Default };

	};


	class WalkState : public State
	{
	public:
		WalkState()
		{
			m_Type = StateType::Walk;
		}

		~WalkState() = default;

		bool HandleInput(CharacterComponent* character, StateType newStateType, int inputType) override;

	};

	class IdleState : public State
	{
	public:

		IdleState()
		{
			m_Type = StateType::Idle;
		}

		~IdleState() = default;

		bool HandleInput(CharacterComponent* character, StateType newStateType, int inputType) override;
	};


}
