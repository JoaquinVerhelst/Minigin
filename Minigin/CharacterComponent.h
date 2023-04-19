#pragma once


#include "Component.h"
#include <glm/glm.hpp>

#include "Subject.h"
#include "GameObject.h"



namespace dae
{
	class State;

	enum class StateType
	{
		Default,
		Walk,
		Idle
	};

	class CharacterComponent : public Component
	{
	public:
		CharacterComponent(GameObject* owner, float walkSpeed = { 50.f }, bool isControlledByPlayer =  false  );
		~CharacterComponent();

		void Update() override;
		void Init() override;
		bool IsRenderer() override { return false; }



		CharacterComponent(const CharacterComponent& other) = delete;
		CharacterComponent(CharacterComponent&& other) = delete;
		CharacterComponent& operator=(const CharacterComponent& other) = delete;
		CharacterComponent& operator=(CharacterComponent&& other) = delete;

		void Walk(int direction, int inputType, StateType newStateType);

		int GetInputID() { return m_InputID; }

		//void CalculatePosition();

		State* GetState() { return m_CurrentState; }
		void SetState(State* newState);

	protected:

		int m_InputID;
		float m_WalkSpeed;

		State* m_CurrentState;


		const int m_GridCellSize = 50;

		// Define the maximum number of cells in the x and y directions
		int m_MaxCellsX;
		int m_MaxCellsY;

	};
}