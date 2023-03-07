#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SimpleRenderComponent.h"
#include <iostream>




dae::GameObject::GameObject()
	:m_Components{}
	, m_Transform{}
	, m_IsDestroyed{ false }
	, m_CanRender{ true }
	, m_LocalPosition{}
	, m_Parent{}
	, m_WorldPosition{}
	, m_PositionIsDirty{}
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
	if (m_Debug)
	{
		//std::cout << "Debug Comonent" << '\n';
	}



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


void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
	SetPositionDirty();
	UpdateWorldPosition();
	UpdateTransform();
}




void dae::GameObject::SetParent(std::shared_ptr<GameObject> parent, bool keepWorldPosition)
{
	if (parent == nullptr)
		SetLocalPosition(GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			SetLocalPosition(GetLocalPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}
	if (m_Parent)
		m_Parent->RemoveChild(shared_from_this());
	m_Parent = parent;
	if (m_Parent)
		m_Parent->AddChild(shared_from_this(), keepWorldPosition);

}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalPosition = pos;
	SetPositionDirty();
}

void dae::GameObject::SetPositionDirty()
{
	m_PositionIsDirty = true;
	for (auto child : m_Children)
	{
		child->SetPositionDirty();
		//child->UpdateWorldPosition();
	}
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_Parent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = m_Parent->GetWorldPosition() + m_LocalPosition;
	}
	m_PositionIsDirty = false;
}


void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	if (child == nullptr)
		return;

	auto it = std::find(m_Children.begin(), m_Children.end(), child);
	if (it != m_Children.end())
	{
		child->SetParent(nullptr, false);
		m_Children.erase(it);
		child->GetWorldPosition();
	}
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> child, bool keepWorldPosition)
{
	if (child == nullptr)
		return;

	m_Children.push_back(child);

	if (keepWorldPosition)
		child->SetLocalPosition(GetLocalPosition() - child->GetParent()->GetWorldPosition());

	child->SetPositionDirty();

}

void dae::GameObject::UpdateTransform()
{
	if (m_Parent)
	{
		const auto& parentPos = m_Parent->GetPosition().GetPosition();
		const auto& localPos = m_Transform.GetPosition();
		m_Transform.SetPosition(parentPos.x + localPos.x, parentPos.y + localPos.y, parentPos.z + localPos.z);
	}

	// Update children
	for (auto child : m_Children)
	{
		child->UpdateTransform();
	}
}
