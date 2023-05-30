#include "HealthBarComponent.h"
#include "Renderer.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include <iostream>


dae::HealthBarComponent::HealthBarComponent(GameObject* owner, const std::string& filePath )
    : HealthBarComponent(owner, filePath, false)
{
}

dae::HealthBarComponent::HealthBarComponent(GameObject* owner, const std::string& filePath, bool isReversed)
    : Component(owner)
    , m_Texture{ nullptr }
    , m_HealthComponent{ nullptr }
    , m_IsReversed{ isReversed}
    ,m_Position{}
{
    m_Texture = std::make_unique<Texture2D>(filePath);

    m_Position.SetPosition(0.f, 0.f, 0.f);


    m_HealthComponent = &GetOwner()->GetComponent<HealthComponent>();


    if (m_HealthComponent)
    {
        m_CurrentHealth = m_HealthComponent->GetHealth();
        m_HealthComponent->addObserver(this);
    }
}

dae::HealthBarComponent::~HealthBarComponent()
{
    if (m_HealthComponent)
    {
        m_HealthComponent->removeAllObservers();
       //delete m_HealthComponent;
        m_HealthComponent = nullptr;
    }

   
}

void dae::HealthBarComponent::Update()
{
}

void dae::HealthBarComponent::Init()
{
}

void dae::HealthBarComponent::Render() const
{
    auto pos = m_Position.GetPosition();
    auto textureSize = m_Texture.get()->GetSize();
    float offset = 5.f;

    if (m_IsReversed)
    {
        for (size_t i = 0; i < m_CurrentHealth; i++)
        {
            Renderer::GetInstance().RenderTexture(*m_Texture, pos.x - textureSize.x * i - offset * i, pos.y);
        }
    }
    else
    {
        for (size_t i = 0; i < m_CurrentHealth; i++)
        {
            Renderer::GetInstance().RenderTexture(*m_Texture, pos.x + textureSize.x * i + offset * i, pos.y);
        }
    }


}

void dae::HealthBarComponent::SetStartPosition(float x, float y)
{
    m_Position.SetPosition(x, y, 0.f);
}

void dae::HealthBarComponent::SetIsReversed(bool isBarReveresd)
{
    m_IsReversed = isBarReveresd;
}

void dae::HealthBarComponent::OnNotify(const GameObject& entity, Event event)
{
    if (event == Event::PlayerHealthChanged && &entity == GetOwner())
    {
        // Update the health bar UI
        m_CurrentHealth = m_HealthComponent->GetHealth();
    }

    if (event == Event::PlayerDie && &entity == GetOwner())
    {
        // World player is dead
        std::cout << "DEAD" << '\n';
    }
}
