#include "FPSComponent.h"

#include "GameObject.h"
#include "SimpleRenderComponent.h"
#include "Time.h"
#include <string>


dae::FPSComponent::FPSComponent(GameObject* owner)
	: Component(owner),
	m_Count{ 0.5f },
	m_UpdateTime{ 0.5f },
	m_FPSText{},
	m_HasRenderComponent{false}
{
}



void dae::FPSComponent::Update()
{
	m_Count += Time::GetInstance().GetDeltaTime();

	if (m_Count >= m_UpdateTime)
	{
		if (m_HasRenderComponent)
		{
			m_FPSText = "FPS: " + std::to_string(static_cast<int>(Time::GetInstance().GetFPS()));
			GetOwner()->GetComponent<SimpleRenderComponent>().SetText(m_FPSText);
		}
		else
		{
			if (GetOwner()->HasComponent<SimpleRenderComponent>())
			{
				m_HasRenderComponent = true;
			}
		}

	}

}
