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
	if (health == -1)
	{
		notify(*GetOwner(), Event::PlayerDie);
	}
	else
	{
		m_CurrentHealth = health;
	}

	notify(*GetOwner(), Event::PlayerHealthChanged);
}
