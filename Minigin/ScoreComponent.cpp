#include "ScoreComponent.h"


void dae::ScoreComponent::Update()
{
}

dae::ScoreComponent::ScoreComponent(GameObject* owner)
	:Component{ owner }
	, m_CurrentScore{ 0 }
{
}



int dae::ScoreComponent::GetScore()
{
	return m_CurrentScore;
}

void dae::ScoreComponent::AddScore(int score)
{
	m_CurrentScore += score;
	notify(*GetOwner(), Event::PlayerScoreChanged);
}

