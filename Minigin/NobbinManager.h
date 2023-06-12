#pragma once


#include "Component.h"
#include "Subject.h"
#include <vector>
#include "GameObject.h"
#include "World.h"

namespace dae
{
	//class GameObject;

	class NobbinManager final : public Component
	{
	public:

		void Update() override;
		void Init() override;
		void Render() const override;
		bool IsRenderer() override { return true; }

		NobbinManager(GameObject* owner, int amountOfNobbin, int gridIndex,  float timeTospawnNobbin = 5.f);
		~NobbinManager();

		NobbinManager(const NobbinManager& other) = delete;
		NobbinManager(NobbinManager&& other) = delete;
		NobbinManager& operator=(const NobbinManager& other) = delete;
		NobbinManager& operator=(NobbinManager&& other) = delete;

		void Reset();
		void Disable();

		std::vector<std::shared_ptr<GameObject>> GetNobbins();
		void DestroyNobbin(std::shared_ptr<GameObject> nobbin);


		void SetNobbinPlayer(std::shared_ptr<GameObject> nobbinPlayer);

		void CalculateNobbins();
		void CalculateNobbinPlayer();


	private:

		void SpawnNobbin();
		bool IsOverlappingWithPlayer(std::shared_ptr<GameObject> nobbin, std::shared_ptr<GameObject> player);
		bool IsOverlappingWithTreasure(std::shared_ptr<GameObject> nobbin);

		float m_Counter;
		float m_TimeToSpawn;
		int m_NobbinAmountLeft;
		int m_GridIndex;
		GridCell* m_GridCell;
		int m_NobbinAmount;

		std::vector<std::shared_ptr<GameObject>> m_Nobbins;
		std::vector<std::shared_ptr<GameObject>> m_Players;
		std::shared_ptr<GameObject> m_NobbinPlayer;
	};
}