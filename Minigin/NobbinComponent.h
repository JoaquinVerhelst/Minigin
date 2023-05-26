#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include "CharacterComponent.h"
#include "Subject.h"

namespace dae
{
	class GameObject;

	class NobbinComponent final : public CharacterComponent
	{
	public:
		void Update() override;
		void Init() override;
		void Render() const override;
		bool IsRenderer() override { return true; }

		NobbinComponent(GameObject* owner, float walksSpeed = { 50.f });
		~NobbinComponent();

		NobbinComponent(const NobbinComponent& other) = delete;
		NobbinComponent(NobbinComponent&& other) = delete;
		NobbinComponent& operator=(const NobbinComponent& other) = delete;
		NobbinComponent& operator=(NobbinComponent&& other) = delete;

		//void HorizontalWalk(int direction) override;
	private:



		void CalculateMovement();

	};
}
