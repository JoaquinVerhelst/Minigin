#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <SDL.h>


namespace dae
{
    //class Texture2D;
    //class string;
    class Font;


    class SimpleRenderComponent : public Component {
    public:

        SimpleRenderComponent(GameObject* owner, const std::string& filePath, bool isbackground = false);
        SimpleRenderComponent(GameObject* owner, const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating);

        ~SimpleRenderComponent();


        void Update() override;
        void Init() override;
        void Render() const override;
        void Render(glm::vec3 position) const;
        bool IsRenderer() override { return true; }

        SimpleRenderComponent(const SimpleRenderComponent& other) = delete;
        SimpleRenderComponent(SimpleRenderComponent&& other) = delete;
        SimpleRenderComponent& operator=(const SimpleRenderComponent& other) = delete;
        SimpleRenderComponent& operator=(SimpleRenderComponent&& other) = delete;

        void SetTexture(const std::string& filePath);
        void SetText(const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating);
        void SetText(const std::string& text);
        std::string GetText();


        std::shared_ptr<Texture2D> GetTexture();
        void UpdateText();

        void SetScale(int scaleX, int scaleY);
        //void SetScale(const glm::vec2& scale); // Not yet Implemented

        void SetAngle(float angle) { m_Angle = angle; }
        float GetAngle() { return m_Angle; }
        void SetFlip(SDL_RendererFlip flip) { m_Flip = flip; }

        void SetAngleAndFlip(float angle, SDL_RendererFlip flip);

        void SetPosition(glm::vec2 pos) { m_Position = pos; }
        glm::vec2 GetPosition() { return m_Position; }

    private:

        bool m_NeedsUpdate;
        bool m_DoOnce;
        bool m_IsBackground;

        std::string m_Text;

        std::shared_ptr<Font> m_Font;

        std::shared_ptr<Texture2D> m_Texture;

        SDL_Rect m_SourceRect;
        int m_ScaleY;
        int m_ScaleX;

        float m_Angle;
        SDL_RendererFlip m_Flip;

        glm::vec2 m_Position;

        //glm::vec2 m_Scale;  // Not yet Implemented

    };


}
