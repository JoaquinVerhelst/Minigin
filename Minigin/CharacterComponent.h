#pragma once


#include "Component.h"
#include <glm/glm.hpp>

#include "Subject.h"
#include "GameObject.h"
#include "Command.h"


namespace dae
{
	class State;

	enum class StateType
	{
		Default,
		HorizontalWalk,
		VerticalWalk,
		Idle
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

		void Walk(int direction, int inputType, StateType newStateType);

		void HorizontalWalk(int direction, Command::InputType inputType, StateType newStateType);
		void VerticalWalk(int direction, Command::InputType inputType, StateType newStateType);




		int GetInputID() { return m_InputID; }

		//void CalculatePosition();

		State* GetState() { return m_CurrentState; }
		void SetState(State* newState);

	protected:

		int m_InputID;
		float m_WalkSpeed;

		State* m_CurrentState;


		glm::vec2 m_CellSize;


	private:

		glm::vec2 CalculateWalk(int direction, float cellSize , float x, float y);
	};
}