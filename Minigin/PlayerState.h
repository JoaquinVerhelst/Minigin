#pragma once

//#include "InputManager.h"
#include "Command.h"



namespace dae
{
	class CharacterComponent;
	enum class Direction;



	enum class PlayerStateType
	{
		Idle,
		HorizontalWalk,
		VerticalWalk,
		Damaged,
		Dead
	};

	class CharacterState
	{
	public:


		CharacterState() = default;
		virtual ~CharacterState() = default;
		virtual void UpdateSprite(GameObject* actor, Direction direction) = 0;
		virtual CharacterState* HandleInput(Command::InputType inputType, PlayerStateType newStateType) = 0;

		virtual void Update(GameObject*, CharacterComponent*) {};
		virtual bool CheckCollision(GameObject*, CharacterComponent*) { return false; };
		virtual void CalculateDirection(GameObject*, CharacterComponent*) {};

		virtual PlayerStateType GetType() { return m_Type; }

	protected:
		PlayerStateType m_Type{ PlayerStateType::Idle };

	};


	class HorizontalWalkState : public CharacterState
	{
	public:
		HorizontalWalkState()
		{
			m_Type = PlayerStateType::HorizontalWalk;
		}

		~HorizontalWalkState() = default;
		void UpdateSprite(GameObject* actor, Direction direction) override;
		CharacterState* HandleInput( Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
		bool CheckCollision(GameObject* actor, CharacterComponent* character) override;
		void CalculateDirection(GameObject* actor , CharacterComponent* character ) override;
	};

	class VerticalWalkState : public CharacterState
	{
	public:
		VerticalWalkState()
		{
			m_Type = PlayerStateType::VerticalWalk;
		}

		~VerticalWalkState() = default;
		void UpdateSprite(GameObject* actor, Direction direction) override;
		CharacterState* HandleInput( Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
		bool CheckCollision(GameObject* actor, CharacterComponent* character) override;
		void CalculateDirection(GameObject* actor, CharacterComponent* character) override;
	};


	class IdleState : public CharacterState
	{
	public:

		IdleState()
		{
			m_Type = PlayerStateType::Idle;
		}

		~IdleState() = default;
		void UpdateSprite(GameObject* actor, Direction direction) override;
		CharacterState* HandleInput( Command::InputType inputType, PlayerStateType newStateType) override;
		bool CheckCollision(GameObject* actor, CharacterComponent* character) override;
	};


	class DamagedState : public CharacterState
	{
	public:

		DamagedState()
		{
			m_Type = PlayerStateType::Damaged;
		}

		~DamagedState() = default;
		void UpdateSprite(GameObject* actor, Direction direction) override;
		CharacterState* HandleInput(Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
	};


	class DeadState : public CharacterState
	{
	public:

		DeadState()
		{
			m_Type = PlayerStateType::Dead;
		}

		~DeadState() = default;
		void UpdateSprite(GameObject* actor, Direction direction) override;
		CharacterState* HandleInput(Command::InputType inputType, PlayerStateType newStateType) override;
		void Update(GameObject* actor, CharacterComponent* character) override;
	};



}
