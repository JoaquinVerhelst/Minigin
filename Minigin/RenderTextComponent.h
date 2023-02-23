#pragma once
#include "Transform.h"
#include "Component.h"
#include "Texture2D.h"
#include <memory>

namespace dae
{
	class Font;


	class RenderTextComponent final : public Component
	{
	public:
		void Update() override;
		void Render() const override;
		void Init() override;
		bool IsRenderer() override { return true; }

		void SetText(const std::string& text);

		explicit RenderTextComponent(const std::string& text, const std::shared_ptr<Font>& font, bool needUpdating);
		~RenderTextComponent() = default;

		RenderTextComponent(const RenderTextComponent& other) = delete;
		RenderTextComponent(RenderTextComponent&& other) = delete;
		RenderTextComponent& operator=(const RenderTextComponent& other) = delete;
		RenderTextComponent& operator=(RenderTextComponent&& other) = delete;

	private:

		bool m_NeedsUpdate;
		bool m_DoOnce;

		std::string m_Text;

		std::shared_ptr<Font> m_Font;

		std::unique_ptr<Texture2D> m_TextTexture;

		
	};
}
