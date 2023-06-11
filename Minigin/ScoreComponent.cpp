#include "ScoreComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"

void dae::ScoreComponent::Update()
{
}

dae::ScoreComponent::ScoreComponent(GameObject* owner)
	:Component{ owner }
	, m_CurrentScore{ 0 }
	, m_EmeraldsInARow{ 0 }
{
}



int dae::ScoreComponent::GetScore()
{
	return m_CurrentScore;
}

void dae::ScoreComponent::AddScore(int score)
{
	m_EmeraldsInARow = 0;
	m_CurrentScore += score;
	notify(*GetOwner(), Event::PlayerScoreChanged);
}

void dae::ScoreComponent::AddEmeraldScore(int score)
{
	++m_EmeraldsInARow;
	m_CurrentScore += score;
	notify(*GetOwner(), Event::PlayerScoreChanged);

	if (m_EmeraldsInARow == 8)
	{
		HealthComponent& comp = GetOwner()->GetComponent<HealthComponent>();
		comp.SetHealth(comp.GetHealth() + 1);

		//magic number -> Add 250 score when 8 emeralds picked up
		m_CurrentScore += 250;
		notify(*GetOwner(), Event::PlayerScoreChanged);
	}
}


// add a event emeraldpicked up