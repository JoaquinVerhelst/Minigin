#pragma once

#include "Component.h"
#include <glm/glm.hpp>

#include "SDL.h"
#include "GameObject.h"
#include "Command.h"


namespace dae
{
	enum class Direction;


	class FireBallComponent final : public Component
	{

	public:





		FireBallComponent(GameObject* owner, GameObject* digger,  glm::vec2 position, Direction direction);
		~FireBallComponent();

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
		glm::vec2 m_Size;
		Direction m_Direction;
		float m_Speed;
		std::vector<std::shared_ptr<GameObject>> m_Nobbins;
		GameObject* m_DiggerOwner;
	};
}
