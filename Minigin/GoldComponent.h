#pragma once


#include "TreasureComponent.h"
#include <glm/glm.hpp>

#include "SDL.h"
#include "Subject.h"
#include "GameObject.h"
#include "Command.h"


namespace dae
{

	class GoldComponent final : public TreasureComponent
	{
	public:
		GoldComponent(GameObject* owner, int gridIndex);

		void Update() override;
		void Init() override;

		bool PickUpTreasure(GameObject* actor) override;

		bool CalculateCollision(GameObject* actor) override;

		GoldComponent(const GoldComponent& other) = delete;
		GoldComponent(GoldComponent&& other) = delete;
		GoldComponent& operator=(const GoldComponent& other) = delete;
		GoldComponent& operator=(GoldComponent&& other) = delete;

	private:

		void CalculatePush();

		int m_CurrentGridIndex;
		int m_FallingDistance;
		float m_WaitToFallTime; 
		float m_WaitToFallCounter;



		//0 is left, 1 is right
		int m_PushDirection;

		bool m_IsFalling;
		bool m_IsBroken;
		bool m_IsPushed;
	};
}

