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

		enum class WorldTypes
		{
			Menu,
			Level,
			HighScore
		};

		~World();
		void Init(int rows, int columns, SDL_Window* window);
		void Render() const;
		void Reset();
		void Update();


		bool IsOverlappingWithWorld(const glm::vec2& position, const glm::vec2& size) const;
		size_t GetOverlappedTreasureIndex(const glm::vec2& position, const glm::vec2& size) const;
		GridCell* GetOverlappedCell(const glm::vec2& position, const glm::vec2& size) const;


		void BreakWorld(GameObject* actor, const glm::vec2& size);

		//returns true if it collides with treasure that cannot be pushed
		bool CheckForTreasure(GameObject* actor, const glm::vec2& size);

		glm::vec2 GetCellSize();

		void UpdateWorld();
		std::vector<GridCell*> GetWorldGrid();


		void PlaceTreasure(std::shared_ptr<GameObject> treasure, int gridIndex);
		void PlaceGameObject(std::shared_ptr<GameObject> gameobject, int gridIndex);
		void PlaceGameObject(GameObject* gameobject, int gridIndex);


		void BreakGridIndexes(std::vector<int> gridIndexes);

		void SetGameMode(GameModeTypes gameMode);

		void ResetAndLoadWorld(int index);

		void PlayerDied();
	private:

		std::vector<GridCell*> m_Grid;
		int m_Rows;
		int m_Columns;
		int m_CellWidth;
		int m_CellHeight;


		std::vector<std::shared_ptr<GameObject>> m_Treasure;

		bool m_UpdateTexture;
		float m_Count = 0;

		GameModeTypes m_CurrentGameMode;

		int m_CurrentWorldIndex;
	};





}


// bij menu kies je de coop, single of multi -> Dan load je lvl 1 gebaseerd op dat, f1 load je lvl2 gebaseerd op gamemode , enzo....

//esc terug naar menu en kies terug gamemode, en load je zo terug....