#include "SimpleRenderComponent.h"
#include <glm/glm.hpp>
#include "Renderer.h"
#include "GameObject.h"

dae::SimpleRenderComponent::SimpleRenderComponent(const std::string& filePath)
	:m_Texture{ nullptr }
{
	m_Texture = std::make_unique<Texture2D>( filePath );
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


//void dae::SimpleRenderComponent::SetScale(const glm::vec2& scale)
//{
//}


