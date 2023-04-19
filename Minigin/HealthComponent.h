#pragma once




#include "Component.h"
#include <glm/glm.hpp>
#include "Subject.h"


namespace dae
{

	class GameObject;

	class HealthComponent final : public Component, public Subject
	{
	public:
		void Update() override;
		bool IsRenderer() override { return false; }

		HealthComponent(GameObject* owner, int health);
		~HealthComponent();

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		int GetHealth();
		void SetHealth(int health);
	
	private:

		int m_CurrentHealth;
	};
}
