#pragma once

#include "Component.h"
#include <glm/glm.hpp>

#include "SDL.h"
#include "Subject.h"
#include "GameObject.h"
#include "Command.h"


namespace dae
{

	class FireBallComponent final : public Component
	{
	public:

		FireBallComponent(GameObject* owner);

		void Update() override;
		void Init() override;
		bool IsRenderer() override { return false; }

		void CalculateMovement();
		void CheckCollision();

		FireBallComponent(const FireBallComponent& other) = delete;
		FireBallComponent(FireBallComponent&& other) = delete;
		FireBallComponent& operator=(const FireBallComponent& other) = delete;
		FireBallComponent& operator=(FireBallComponent&& other) = delete;


	private:


	};
}
