#pragma once

#include "Observer.h"
#include "Component.h"
#include "Texture2D.h"
#include <memory>
#include "Transform.h"

namespace dae
{
    class HealthComponent;


    class HealthBarComponent : public Component, public Observer
    {
    public:

        explicit HealthBarComponent(GameObject* owner,  const std::string& filePath);
        explicit HealthBarComponent(GameObject* owner, const std::string& filePath, bool isReversed);
        ~HealthBarComponent();

        void Update() override;
        void Init() override;
        void Render() const override;
        bool IsRenderer() override { return true; }

        void SetStartPosition(float x, float y);
        void SetIsReversed(bool isBarReveresd);

        void OnNotify(const GameObject& entity, Event event) override;


    private:

        int m_CurrentHealth;
        bool m_IsReversed;
        HealthComponent* m_HealthComponent;
        std::unique_ptr<Texture2D> m_Texture;
        Transform m_Position;

    };
}


