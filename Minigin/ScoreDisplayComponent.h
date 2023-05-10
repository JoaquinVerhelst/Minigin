#pragma once


#include "Observer.h"
#include "Component.h"
#include "Texture2D.h"
#include <memory>
#include "Transform.h"
#include "SimpleRenderComponent.h"

#include "Font.h"
#include <string>

namespace dae
{
    class ScoreComponent;


    class ScoreDisplayComponent : public Component, public Observer
    {
    public:

        explicit ScoreDisplayComponent(GameObject* owner, const std::shared_ptr<Font>& font);
        ~ScoreDisplayComponent();

        void Update() override;

        ScoreDisplayComponent(const ScoreDisplayComponent& other) = delete;
        ScoreDisplayComponent(ScoreDisplayComponent&& other) = delete;
        ScoreDisplayComponent& operator=(const ScoreDisplayComponent& other) = delete;
        ScoreDisplayComponent& operator=(ScoreDisplayComponent&& other) = delete;


        void Render() const override;
        bool IsRenderer() override { return true; }

        void SetStartPosition(float x, float y);

        void OnNotify(const GameObject& entity, Event event) override;
        //void UpdateText();


    private:

        int m_CurrentScore;

        ScoreComponent* m_ScoreComponent;

        std::unique_ptr<SimpleRenderComponent> m_SimpleRenderComponent;

        Transform m_Position;

    };
}

