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

		void Render() const override;
		void Init() override;

		bool IsRenderer() override { return false; }


		FPSComponent();
		~FPSComponent() = default;

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

	private:

		std::string m_FPSText{};
	};
}