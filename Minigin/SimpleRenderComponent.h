#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <memory>
#include <string>

namespace dae
{
    class Texture2D;
    class string;

    class SimpleRenderComponent : public Component {
    public:
        SimpleRenderComponent(const std::string& filePath);
        ~SimpleRenderComponent();


        void Update() override;
        void Init() override;
        void Render() const override;
        bool IsRenderer() override { return true; }

        SimpleRenderComponent(const SimpleRenderComponent& other) = delete;
        SimpleRenderComponent(SimpleRenderComponent&& other) = delete;
        SimpleRenderComponent& operator=(const SimpleRenderComponent& other) = delete;
        SimpleRenderComponent& operator=(SimpleRenderComponent&& other) = delete;

        void SetTexture(const std::string& filePath);

        //void SetScale(const glm::vec2& scale); // Not yet Implemented

    private:

        std::unique_ptr<Texture2D> m_Texture;
        //glm::vec2 m_Scale;  // Not yet Implemented
    };


}
