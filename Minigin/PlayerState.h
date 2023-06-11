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
		VerticalWalk,
		Damaged,
		Dead

	};

	class PlayerState
	{
	public:


		PlayerState() = default;
		virtual ~PlayerState() = default;
		virtual void UpdateSprite(GameObject* actor, int direction) = 0;
		virtual PlayerState* HandleInput(Command::InputType inputType, PlayerStateType newStateType) = 0;

		virtual void Update(GameObject*, CharacterComponent*) {};
		virtual bool CheckCollision(GameObject*, CharacterComponent*) { return false; };
		virtual void CalculateDirection(GameObject*, CharacterComponent*) {};

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
		void UpdateSprite(GameObject* actor, int direction) override;
		PlayerState* HandleInput( Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
		bool CheckCollision(GameObject* actor, CharacterComponent* character) override;
		void CalculateDirection(GameObject* actor , CharacterComponent* character ) override;
	};

	class VerticalWalkState : public PlayerState
	{
	public:
		VerticalWalkState()
		{
			m_Type = PlayerStateType::VerticalWalk;
		}

		~VerticalWalkState() = default;
		void UpdateSprite(GameObject* actor, int direction) override;
		PlayerState* HandleInput( Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
		bool CheckCollision(GameObject* actor, CharacterComponent* character) override;
		void CalculateDirection(GameObject* actor, CharacterComponent* character) override;
	};


	class IdleState : public PlayerState
	{
	public:

		IdleState()
		{
			m_Type = PlayerStateType::Idle;
		}

		~IdleState() = default;
		void UpdateSprite(GameObject* actor, int direction) override;
		PlayerState* HandleInput( Command::InputType inputType, PlayerStateType newStateType) override;
		bool CheckCollision(GameObject* actor, CharacterComponent* character) override;
	};


	class DamagedState : public PlayerState
	{
	public:

		DamagedState()
		{
			m_Type = PlayerStateType::Damaged;
		}

		~DamagedState() = default;
		void UpdateSprite(GameObject* actor, int direction) override;
		PlayerState* HandleInput(Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
	};


	class DeadState : public PlayerState
	{
	public:

		DeadState()
		{
			m_Type = PlayerStateType::Dead;
		}

		~DeadState() = default;
		void UpdateSprite(GameObject* actor, int direction) override;
		PlayerState* HandleInput(Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
	};

}
