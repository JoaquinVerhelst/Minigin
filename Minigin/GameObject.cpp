#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SimpleRenderComponent.h"





dae::GameObject::GameObject()
	:m_Components{}
	, m_Transform{}
	, m_IsDestroyed{ false }
	, m_CanRender{ true }
{
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	const auto count = m_Components.size();

	for (size_t i = 0; i < count; i++)
	{
		m_Components[i]->Update();
	}
}

void dae::GameObject::FixedUpdate()
{
}

void dae::GameObject::Render() const
{	
	//this is a temporary check for renderers -> will prob change this
	if (m_CanRender)
	{
		const auto count = m_Components.size();

		for (size_t i = 0; i < count; i++)
		{
			if (m_Components[i]->IsRenderer())
			{
				m_Components[i]->Render();
			}
		}

	}
}

void dae::GameObject::Init()
{

}

//void dae::GameObject::SetTexture(const std::string& filename)
//{
//	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
//}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
