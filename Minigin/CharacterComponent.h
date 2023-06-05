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
		void SetDirection(int direction);
		int GetDirection();

		glm::ivec2 GetCellSize();

		int GetInputID() { return m_InputID; }

		void CheckBorders();

		PlayerState* GetState() { return m_CurrentState; }
		void SetState(PlayerState* newState);

		glm::vec2 CalculateWalk(int direction, int cellSize, float x, float y);
		GridCell* GetCurrentCell() { return m_CurrentCell; }

		virtual void GetDamaged();
		virtual void Shoot();
		virtual void Respawn();
		virtual void UpdateDeath();

		virtual void CalculateMovement() {};
		virtual void CalculateCell() {};
	protected:

		int m_Direction;
		int m_InputID;
		float m_WalkSpeed;
		bool m_ControlledByPlayer;


		PlayerState* m_CurrentState;


		glm::ivec2 m_CellSize;

		GridCell* m_CurrentCell;



	private:


		//glm::vec2 CheckForCollision(int direction, float cellSize, float x, float y);
	};
}