#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include "CharacterComponent.h"
#include "Subject.h"

namespace dae
{
	class GameObject;

	class DiggerComponent final : public CharacterComponent
	{
	public:
		void Update() override;
		void Init() override;
		void Render() const override;
		bool IsRenderer() override { return true; }

		DiggerComponent(GameObject* owner, float walksSpeed = {50.f});
		~DiggerComponent() = default;

		DiggerComponent(const DiggerComponent& other) = delete;
		DiggerComponent(DiggerComponent&& other) = delete;
		DiggerComponent& operator=(const DiggerComponent& other) = delete;
		DiggerComponent& operator=(DiggerComponent&& other) = delete;


	private:






	};
}