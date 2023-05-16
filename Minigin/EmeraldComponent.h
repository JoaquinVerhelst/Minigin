#pragma once

#include "TreasureComponent.h"
#include <glm/glm.hpp>

#include "SDL.h"
#include "Subject.h"
#include "GameObject.h"
#include "Command.h"


namespace dae
{

	class EmeraldComponent final : public TreasureComponent
	{
	public:
		EmeraldComponent(GameObject* owner);

		void Update() override;
		void Init() override;

		bool PickUpTreasure(GameObject* actor) override;
		bool CalculateCollision(GameObject* actor) override;


		EmeraldComponent(const EmeraldComponent& other) = delete;
		EmeraldComponent(EmeraldComponent&& other) = delete;
		EmeraldComponent& operator=(const EmeraldComponent& other) = delete;
		EmeraldComponent& operator=(EmeraldComponent&& other) = delete;

	};
}


