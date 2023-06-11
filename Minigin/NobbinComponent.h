#pragma once

#include "Component.h"

#include "CharacterComponent.h"


namespace dae
{
	class GameObject;

	class NobbinComponent final : public CharacterComponent
	{
	public:

		struct NobbinSprites
		{
			std::string enemySprite;
			std::string rageSprite;
		};


		void Update() override;
		void Init() override;
		void Render() const override;
		bool IsRenderer() override { return true; }

		NobbinComponent(GameObject* owner, float walksSpeed = { 50.f }, bool isControlledByPlayer = false);
		~NobbinComponent();

		NobbinComponent(const NobbinComponent& other) = delete;
		NobbinComponent(NobbinComponent&& other) = delete;
		NobbinComponent& operator=(const NobbinComponent& other) = delete;
		NobbinComponent& operator=(NobbinComponent&& other) = delete;

		//void HorizontalWalk(int direction) override;

		void UseSpecialty() override;

		NobbinSprites GetSprites() { return m_Sprites; }
		void SetSprites(NobbinSprites sprites) { m_Sprites = sprites; }

	private:

		bool CalculateBounceBack();
		void CalculateMovement() override;
		void CalculateCell() override;

		float m_Counter;
		float m_UpdateDirectionTime;

		NobbinSprites m_Sprites;

		float m_RageCounter;
		float m_RageTime;
		bool m_StartRage;

		float m_CoolDownTime;
		bool m_CanRage;


		bool doonce = true;
	};
}
