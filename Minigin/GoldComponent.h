#pragma once


#include "TreasureComponent.h"
#include <glm/glm.hpp>

#include "SDL.h"
#include "Subject.h"
#include "GameObject.h"
#include "Command.h"

#include "GoldStateMachine.h"


namespace dae
{

	class GoldComponent final : public TreasureComponent
	{
	public:
		GoldComponent(GameObject* owner, int gridIndex, float pushspeed = {50.f}, float fallspeed = {120.f});
		~GoldComponent();

		void Update() override;
		void FixedUpdate() override;
		void Init() override;

		bool PickUpTreasure(GameObject* actor) override;

		bool HandleCollision(GameObject* actor) override;


		GoldComponent(const GoldComponent& other) = delete;
		GoldComponent(GoldComponent&& other) = delete;
		GoldComponent& operator=(const GoldComponent& other) = delete;
		GoldComponent& operator=(GoldComponent&& other) = delete;


		void CalculateFall();
		void CheckForFalling();

		void PushRight();
		void PushLeft();

		int GetPushDirection() { return m_PushDirection; }
		int GetCurrentGridIndex() { return m_CurrentGridIndex; }

		void SetState(GoldState* newState);

		float GetPushSpeed() { return m_PushSpeed; }
		float GetFallSpeed() { return m_FallSpeed; }

		GoldStateType GetStateType() { return m_CurrentState->GetType(); }

	private:



		int m_CurrentGridIndex;
		int m_FallingDistance;
		float m_WaitToFallTime; 
		float m_WaitToFallCounter;
		GoldState* m_CurrentState;
		


		//0 is left, 1 is right
		int m_PushDirection;

		float m_PushSpeed;
		float m_FallSpeed;
	};
}

