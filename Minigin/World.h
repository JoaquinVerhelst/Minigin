#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include "GameObject.h"

namespace dae
{
	struct LevelInfo;

	struct GridCell
	{
		int id;
		glm::vec2 position;
		bool isCellBroken;
	};



	class World final : public Singleton<World>
	{


	public:

		enum class GameModeTypes
		{
			SinglePlayer,
			Coop,
			Versus
		};


		~World();
		void Init(int rows, int columns, SDL_Window* window);
		void Render() const;
		void Reset();
		void Update();


		bool IsOverlappingWithWorld(const glm::vec2& position, const glm::vec2& size) const;
		size_t GetOverlappedTreasureIndex(const glm::vec2& position, const glm::vec2& size) const;


		GridCell* GetOverlappedUnbrokenCell(const glm::vec2& position, const glm::vec2& size) const;
		GridCell* GetOverlappedCell(const glm::vec2& position, const glm::vec2& size) const;



		void BreakWorld(GameObject* actor, const glm::vec2& size);

		//returns true if it collides with treasure that cannot be pushed
		bool CheckForTreasure(GameObject* actor, const glm::vec2& size);
		GameObject* GetIsOverlappingTreasure(const glm::vec2& position, const glm::vec2& size) const;



		glm::vec2 GetCellSize();

		void UpdateWorld();
		std::vector<GridCell*> GetWorldGrid();





		void BreakGridIndexes(std::vector<int> gridIndexes);

		void SetGameMode(GameModeTypes gameMode);
		GameModeTypes GetGameMode() { return m_CurrentGameMode; }

		void ResetAndLoadWorld(int index);
		void ResetGameMode();
		void ResetLevel();
		void PlayerDied(GameObject* player);

		void PlaceGameObject(std::shared_ptr<GameObject> gameobject, int gridIndex);
		void PlaceGameObject(GameObject* gameobject, int gridIndex);
		void PlaceEnemy(GameObject* gameobject, int gridIndex);
		std::vector<std::shared_ptr<GameObject>> GetPlayers() { return m_Players; }

		void LoadSinglePlayer();
		void LoadCoop();
		void LoadVersus();

		bool IsGameLost() { return m_GameOver; }
		void SetGameLost(bool resetGame) { m_GameOver = resetGame; }
	private:


		void PlaceTreasure(std::shared_ptr<GameObject> treasure, int gridIndex);
		void LoadNobbinManager();



		std::vector<std::shared_ptr<GameObject>> m_Players;
		std::shared_ptr<GameObject> m_NobbinPlayer;


		std::vector<GridCell*> m_Grid;
		int m_Rows;
		int m_Columns;
		int m_CellWidth;
		int m_CellHeight;

		int m_PlayersDead;
		bool m_GameOver;

		std::vector<std::shared_ptr<GameObject>> m_Treasure;

		bool m_UpdateTexture;
		float m_Count = 0;

		GameModeTypes m_CurrentGameMode;

		int m_CurrentWorldIndex;

		int m_Emeralds;
	};





}


// bij menu kies je de coop, single of multi -> Dan load je lvl 1 gebaseerd op dat, f1 load je lvl2 gebaseerd op gamemode , enzo....

//esc terug naar menu en kies terug gamemode, en load je zo terug....