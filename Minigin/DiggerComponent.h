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

		struct PlayerSprites
		{
			std::string playerSprite;
			std::string deathSprite;
			std::string fireBallSprite;
			std::string reloadingSprite;
		};



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


		void GetDamaged() override;
		void UseSpecialty() override;
		void Respawn() override;
		void UpdateDeath() override;
		void SetIsDead() override;
		PlayerSprites GetSprites() { return m_Sprites; }
		void SetSprites(PlayerSprites sprites) { m_Sprites = sprites; }
	private:

		void SpawnFireBall();

		bool m_IsDamaged;
		float m_DeathCounter;
		float m_DeathTime;
		std::shared_ptr<GameObject> m_FireBall;

		PlayerSprites m_Sprites;

		float m_ShootCount;
		float m_ShootDelay;
		bool m_CanShoot;
	};
}