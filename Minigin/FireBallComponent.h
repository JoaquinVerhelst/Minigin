#pragma once

#include "Component.h"
#include <glm/glm.hpp>

#include "SDL.h"
#include "GameObject.h"
#include "Command.h"


namespace dae
{

	class FireBallComponent final : public Component
	{

	public:

		enum class Direction
		{
			Up,
			Down,
			Left,
			Right
		};



		FireBallComponent(GameObject* owner, glm::vec2 position, Direction direction);

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

		glm::vec2 m_Position;
		Direction m_Direction;
		float m_Speed;
	};
}
