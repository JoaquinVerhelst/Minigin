
#include <SDL_ttf.h>
#include "RenderTextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "GameObject.h"
#include "Time.h"
#include <string>

using namespace dae;


dae::RenderTextComponent::RenderTextComponent(const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating)
	: m_NeedsUpdate(needUpdating),
	m_Text(text),
	m_Font(font),
	m_TextTexture(nullptr),
	m_DoOnce{ true }

{ }

void dae::RenderTextComponent::Update()
{


	if (m_NeedsUpdate || m_DoOnce)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now

		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
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
		m_TextTexture = std::make_unique<Texture2D>( texture );

		m_DoOnce = false;
	}
	

}

void dae::RenderTextComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
		const auto& pos = GetOwner()->GetPosition().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}

}

void dae::RenderTextComponent::Init()
{
}

void dae::RenderTextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}





