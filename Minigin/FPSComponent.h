#pragma once

#include "Transform.h"
#include "Component.h"
#include "Texture2D.h"
#include <memory>

namespace dae
{
	class Font;


	class FPSComponent final : public Component
	{
	public:
		void Update() override;
		bool IsRenderer() override { return false; }

		FPSComponent(GameObject* owner);
		~FPSComponent() = default;

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;


		void SetUpdateTime(float time) { m_UpdateTime = time; }
	private:

		std::string m_FPSText{};
		bool m_HasRenderComponent;

		float m_Count;
		float m_UpdateTime;
	};
}