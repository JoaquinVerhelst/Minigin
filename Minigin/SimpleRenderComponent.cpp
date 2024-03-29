#include "SimpleRenderComponent.h"

#include "Renderer.h"
#include "GameObject.h"
#include "Font.h"
#include <SDL_ttf.h>
#include "Time.h"

#include "ShapeComponent.h"
#include "World.h"

dae::SimpleRenderComponent::SimpleRenderComponent(GameObject* owner, const std::string& filePath, bool isBackground)
	: Component(owner),
	m_Texture{ nullptr },
	m_NeedsUpdate(false),
	m_Text(""),
	m_Font(nullptr),
	m_DoOnce{ true },
	m_SourceRect{},
	m_ScaleX{1},
	m_ScaleY{ 1 },
	m_Angle{ 0 },
	m_Flip{ SDL_FLIP_NONE},
	m_Position{0,0},
	m_IsBackground{ isBackground }
{
	m_Texture = std::make_unique<Texture2D>(filePath);


	m_SourceRect = { 0,0, m_Texture->GetSize().x, m_Texture->GetSize().y };

	GetOwner()->SetShape(glm::ivec2(m_Texture->GetSize().x, m_Texture->GetSize().y));
	//GetOwner()->GetComponent<ShapeComponent>().SetShape(glm::ivec2( m_Texture->GetSize().x, m_Texture->GetSize().y));

}

dae::SimpleRenderComponent::SimpleRenderComponent(GameObject* owner, const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating)
	: Component(owner),
	m_NeedsUpdate(needUpdating),
	m_Text(text),
	m_Font(font),
	m_Texture(nullptr),
	m_DoOnce{ true },
	m_SourceRect{},
	m_ScaleX{ 1 },
	m_ScaleY{ 1 },
	m_Angle{ 0 },
	m_Flip{ SDL_FLIP_NONE },
	m_Position{ 0,0 },
	m_IsBackground{ false  }

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


	if (!m_Text.empty())
	{
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x + m_Position.x, pos.y + m_Position.y);
	}
	if (!m_IsBackground)
	{

		SDL_Rect destRect = { static_cast<int>(pos.x), static_cast<int>(pos.y), m_SourceRect.w * m_ScaleX, m_SourceRect.h * m_ScaleY };
		SDL_Rect sourceRect = m_SourceRect;

		Renderer::GetInstance().RenderTexture(m_Texture->GetSDLTexture(), &sourceRect, &destRect, m_Angle, nullptr, m_Flip);

	}

	
}

void dae::SimpleRenderComponent::Render(glm::vec3 position) const
{
	Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y);
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

std::string dae::SimpleRenderComponent::GetText()
{
	return m_Text;
}

std::shared_ptr<dae::Texture2D> dae::SimpleRenderComponent::GetTexture()
{
	return m_Texture;
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
	m_Texture = std::make_unique<Texture2D>(texture);
}



void dae::SimpleRenderComponent::SetScale(int scaleX, int scaleY)
{
	m_ScaleX = scaleX;
	m_ScaleY = scaleY;
}

void dae::SimpleRenderComponent::SetAngleAndFlip(float angle, SDL_RendererFlip flip)
{
	m_Angle = angle;
	m_Flip = flip;
}


//void dae::SimpleRenderComponent::SetScale(const glm::vec2& scale)
//{
//}


