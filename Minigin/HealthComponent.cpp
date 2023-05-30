#include "HealthComponent.h"

void dae::HealthComponent::Update()
{
}

dae::HealthComponent::HealthComponent(GameObject* owner, int health)
	:Component(owner)
	,m_CurrentHealth(health)
{
}

dae::HealthComponent::~HealthComponent()
{
	//removeAllObservers();
}

int dae::HealthComponent::GetHealth()
{
	return m_CurrentHealth;
}

void dae::HealthComponent::SetHealth(int health)
{
	m_CurrentHealth = health;
	notify(*GetOwner(), Event::PlayerHealthChanged);

	if (m_CurrentHealth == 0)
	{
		notify(*GetOwner(), Event::PlayerDie);
	}
}
