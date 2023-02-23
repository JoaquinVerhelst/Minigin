#include "FPSComponent.h"

#include "GameObject.h"
#include "RenderTextComponent.h"
#include "Time.h"
#include <string>


dae::FPSComponent::FPSComponent()
	:m_FPSText{}
{
}



void dae::FPSComponent::Update()
{
	if (GetOwner()->HasComponent<RenderTextComponent>())
	{
		m_FPSText = "FPS: " + std::to_string(static_cast<int>(Time::GetInstance().GetFPS()));
		GetOwner()->GetComponent<RenderTextComponent>().SetText(m_FPSText);
	}

}

void dae::FPSComponent::Render() const
{
}

void dae::FPSComponent::Init()
{
}

