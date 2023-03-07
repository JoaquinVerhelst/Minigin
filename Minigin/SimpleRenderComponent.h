#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <memory>
#include <string>

namespace dae
{
    class Texture2D;
    class string;
    class Font;


    class SimpleRenderComponent : public Component {
    public:
        explicit SimpleRenderComponent(GameObject* owner, const std::string& filePath);
        explicit SimpleRenderComponent(GameObject* owner, const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating);
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
        void SetText(const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating);
        void SetText(const std::string& text);

        void UpdateText();


        //void SetScale(const glm::vec2& scale); // Not yet Implemented

    private:

        bool m_NeedsUpdate;
        bool m_DoOnce;

        std::string m_Text;

        std::shared_ptr<Font> m_Font;

        std::shared_ptr<Texture2D> m_Texture;


        //glm::vec2 m_Scale;  // Not yet Implemented

    };


}
