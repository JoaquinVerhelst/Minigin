#pragma once



#include "Component.h"
#include <glm/glm.hpp>

#include "SDL.h"
#include "Subject.h"
#include "GameObject.h"
#include "Command.h"


namespace dae
{

	class TreasureComponent : public Component
	{
	public:
		TreasureComponent(GameObject* owner);

		virtual void Update() = 0;
		virtual void Init() = 0;
		bool IsRenderer() override { return false; }

		virtual bool PickUpTreasure(GameObject* actor) = 0;
		virtual bool CalculateCollision(GameObject* actor) = 0;

		TreasureComponent(const TreasureComponent& other) = delete;
		TreasureComponent(TreasureComponent&& other) = delete;
		TreasureComponent& operator=(const TreasureComponent& other) = delete;
		TreasureComponent& operator=(TreasureComponent&& other) = delete;

	protected:


	private:




	};
}
