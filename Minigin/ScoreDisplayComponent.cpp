#include "ScoreDisplayComponent.h"

#include "ScoreComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include <SDL_ttf.h>


dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner, const std::shared_ptr<Font>& font)
	:Component(owner)
	,m_ScoreComponent{nullptr}
	,m_CurrentScore{0}
	,m_Position{}
{

    m_Position.SetPosition(0.f, 0.f, 0.f);

    m_ScoreComponent = &GetOwner()->GetComponent<ScoreComponent>();
	m_SimpleRenderComponent = std::make_unique<SimpleRenderComponent>(GetOwner(), "Score ", font, false);

    if (m_ScoreComponent)
    {
        m_CurrentScore = m_ScoreComponent->GetScore();
        m_ScoreComponent->addObserver(this);
    }

	m_SimpleRenderComponent->UpdateText();
}

dae::ScoreDisplayComponent::~ScoreDisplayComponent()
{
	if (m_ScoreComponent)
	{
		m_ScoreComponent->removeAllObservers();
		m_ScoreComponent = nullptr;
	}


}

void dae::ScoreDisplayComponent::Update()
{
}

void dae::ScoreDisplayComponent::Render() const
{
	m_SimpleRenderComponent->Render(m_Position.GetPosition());
}

void dae::ScoreDisplayComponent::SetStartPosition(float x, float y)
{
	m_Position.SetPosition(x, y, 0.f);
}

void dae::ScoreDisplayComponent::OnNotify(const GameObject& entity, Event event)
{
    if (event == Event::PlayerScoreChanged && &entity == GetOwner())
    {
        // Update the health bar UI
        m_CurrentScore = m_ScoreComponent->GetScore();
		m_SimpleRenderComponent->SetText("Score " + std::to_string(m_CurrentScore));
    }
}
