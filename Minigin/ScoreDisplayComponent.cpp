#include "ScoreDisplayComponent.h"

#include "ScoreComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include <SDL_ttf.h>


dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner, const std::shared_ptr<Font>& font)
	:Component(owner)
	, m_Font{font}
	,m_ScoreComponent{nullptr}
	,m_CurrentScore{0}
	,m_Position{}
	,m_Texture{nullptr}
	,m_Text{"Score"}
{

    m_Position.SetPosition(0.f, 0.f, 0.f);


    m_ScoreComponent = &GetOwner()->GetComponent<ScoreComponent>();


    if (m_ScoreComponent)
    {
        m_CurrentScore = m_ScoreComponent->GetScore();
        m_ScoreComponent->addObserver(this);
    }

	UpdateText();
}

dae::ScoreDisplayComponent::~ScoreDisplayComponent()
{
	if (m_ScoreComponent)
	{
		m_ScoreComponent = nullptr;
	}
}

void dae::ScoreDisplayComponent::Update()
{
}

void dae::ScoreDisplayComponent::Render() const
{
	auto pos = m_Position.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
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
		UpdateText();
    }
}

void dae::ScoreDisplayComponent::UpdateText()
{
	const SDL_Color color = { 255,255,255 }; // only white text is supported now


	std::string fulltext = m_Text + std::to_string(m_CurrentScore);

	const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), fulltext.c_str(), color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_Texture = std::make_unique<Texture2D>(texture);
}
