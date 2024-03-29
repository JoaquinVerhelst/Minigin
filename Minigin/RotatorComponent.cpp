#include "RotatorComponent.h"
#include "GameObject.h"
#include "Time.h"


dae::RotatorComponent::RotatorComponent(GameObject* owner, const glm::vec2& center, float radius, float speed)
    : dae::Component(owner)
    , m_Center(center)
    , m_Radius(radius)
    , m_Speed(speed)
    , m_Angle(0.0f)
{
}


void dae::RotatorComponent::Update()
{
    RotatorLogic();
}


void dae::RotatorComponent::RotatorLogic()
{
    m_Angle += m_Speed * Time::GetInstance().GetDeltaTime();

    if (m_Angle >= 360.f)
    {
        m_Angle -= 360.f;
    }

    glm::vec2 offset = glm::vec2(std::cos(m_Angle), std::sin(m_Angle)) * m_Radius;
    GetOwner()->SetPosition(m_Center.x + offset.x, m_Center.y + offset.y);
}

