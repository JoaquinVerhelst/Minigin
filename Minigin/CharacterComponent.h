#pragma once


#include "Component.h"
#include <glm/glm.hpp>

#include "Subject.h"
#include "GameObject.h"
#include "Command.h"
#include "World.h"
#include "PlayerState.h"

namespace dae
{
	//class State;


	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};


	class CharacterComponent : public Component
	{
	public:




		CharacterComponent(GameObject* owner, float walkSpeed = { 50.f }, bool isControlledByPlayer =  false  );
		~CharacterComponent();

		void Update() override;
		void Render() const override;
		void Init() override;
		bool IsRenderer() override { return true; }



		CharacterComponent(const CharacterComponent& other) = delete;
		CharacterComponent(CharacterComponent&& other) = delete;
		CharacterComponent& operator=(const CharacterComponent& other) = delete;
		CharacterComponent& operator=(CharacterComponent&& other) = delete;

		void HandleInput(Command::InputType inputType, PlayerStateType newStateType);
		//virtual void HorizontalWalk(int direction);
		//void VerticalWalk(int direction);

		glm::vec3 GetPosition();
		void SetDirection(Direction direction);
		Direction GetDirection();



		glm::ivec2 GetCellSize();

		int GetInputID() { return m_InputID; }

		bool CheckBorders();

		CharacterState* GetState() { return m_CurrentState; }


		glm::vec2 CalculateWalk(int direction, int cellSize, float x, float y);
		GridCell* GetCurrentCell() { return m_CurrentCell; }

		virtual void GetDamaged();
		virtual void UseSpecialty() {};
		virtual void Respawn();
		virtual void UpdateDeath();
		virtual void SetIsDead() {};

		virtual void CalculateMovement() {};
		virtual void CalculateCell() {};
	protected:

		void SetState(CharacterState* newState);


		Direction m_Direction;
		int m_InputID;
		float m_WalkSpeed;
		bool m_ControlledByPlayer;
		bool m_IsDead;

		CharacterState* m_CurrentState;


		glm::ivec2 m_CellSize;
		glm::vec2 m_WindowSize;

		GridCell* m_CurrentCell;



	private:


		//glm::vec2 CheckForCollision(int direction, float cellSize, float x, float y);
	};
}