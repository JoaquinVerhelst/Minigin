#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "GameObject.h"

namespace dae
{


	struct GridCell
	{
		glm::vec2 position;
		bool isCellBroken;
	};


	class World final : public Singleton<World>
	{


	public:


		~World();
		void Init(int rows, int columns, SDL_Window* window);
		void Render() const;
		void ResetGrid();
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

	private:

		int m_Rows;
		int m_Columns;
		int m_CellWidth;
		int m_CellHeight;

		std::vector<std::shared_ptr<GameObject>> m_Treasure;
		std::vector<GridCell*> m_Grid;
		bool m_UpdateTexture;

		float m_Count = 0;


	};


}
