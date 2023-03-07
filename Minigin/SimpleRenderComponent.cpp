#include "SimpleRenderComponent.h"
#include <glm/glm.hpp>
#include "Renderer.h"
#include "GameObject.h"
#include "Font.h"
#include <SDL_ttf.h>
#include "Time.h"

dae::SimpleRenderComponent::SimpleRenderComponent(GameObject* owner, const std::string& filePath)
	: Component(owner),
	m_Texture{ nullptr },
	m_NeedsUpdate(false),
	m_Text(""),
	m_Font(nullptr),
	m_DoOnce{ true }
{
	m_Texture = std::make_unique<Texture2D>( filePath );
}

dae::SimpleRenderComponent::SimpleRenderComponent(GameObject* owner, const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating)
	: Component(owner),
	m_NeedsUpdate(needUpdating),
	m_Text(text),
	m_Font(font),
	m_Texture(nullptr),
	m_DoOnce{ true }
{
	UpdateText();
}

dae::SimpleRenderComponent::~SimpleRenderComponent()
{
}

void dae::SimpleRenderComponent::Update()
{

}

void dae::SimpleRenderComponent::Init()
{
}

void dae::SimpleRenderComponent::Render() const
{
	const auto& pos = GetOwner()->GetPosition().GetPosition();


	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	
}

void dae::SimpleRenderComponent::SetTexture(const std::string& filePath)
{
	m_Texture = std::make_unique<Texture2D>(filePath);
}

void dae::SimpleRenderComponent::SetText(const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating)
{
	m_Text = text;
	m_Font = font;
	m_NeedsUpdate = needUpdating;
	UpdateText();
}

void dae::SimpleRenderComponent::SetText(const std::string& text)
{
	m_Text = text;
	UpdateText();
}

void dae::SimpleRenderComponent::UpdateText()
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
	m_Texture = std::make_shared<Texture2D>(texture);
}


//void dae::SimpleRenderComponent::SetScale(const glm::vec2& scale)
//{
//}


