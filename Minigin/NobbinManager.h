#pragma once


#include "Component.h"
#include <vector>

namespace dae
{
	class GameObject;

	class NobbinManager final : public Component
	{
	public:

		void Update() override;
		void Init() override;
		void Render() const override;
		bool IsRenderer() override { return false; }

		NobbinManager(GameObject* owner, int amountOfNobbin, float timeTospawnNobbin = 4.f);
		~NobbinManager();

		NobbinManager(const NobbinManager& other) = delete;
		NobbinManager(NobbinManager&& other) = delete;
		NobbinManager& operator=(const NobbinManager& other) = delete;
		NobbinManager& operator=(NobbinManager&& other) = delete;


	private:

		void SpawnNobbin();
		bool IsOverlappingWithPlayer();

		float m_Counter;
		float m_TimeToSpawn;

		int m_AmountofNobbinLeft;

		std::vector<GameObject> m_Nobbins;
		std::vector<GameObject> m_Players;
	};
}