#pragma once

#include "Component.h"
#include <glm/glm.hpp>



namespace dae
{
	class GameObject;

	class DiggerComponent final : public Component
	{
	public:
		void Update() override;
		void Render() const override;
		void Init() override;
		bool IsRenderer() override { return false; }

		DiggerComponent(GameObject* owner, float walksSpeed = {50.f});
		~DiggerComponent() = default;

		DiggerComponent(const DiggerComponent& other) = delete;
		DiggerComponent(DiggerComponent&& other) = delete;
		DiggerComponent& operator=(const DiggerComponent& other) = delete;
		DiggerComponent& operator=(DiggerComponent&& other) = delete;

		void Walk(int direction);

		int GetInputID() { return m_InputID; }

	private:

		int m_InputID;
		float m_WalkSpeed;
		//InputManager* m_InputManager;

	};
}