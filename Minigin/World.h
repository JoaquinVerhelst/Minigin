#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>


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

		GridCell* GetOverlappedCell(const glm::vec2& position, const glm::vec2& size) const;

		void BreakWorld(const glm::vec2& position, const glm::vec2& size);

		glm::vec2 GetCellSize();

		void UpdateWorld();


	private:

		int m_Rows;
		int m_Columns;
		int m_CellWidth;
		int m_CellHeight;
		std::vector<GridCell*> m_Grid;
		bool m_UpdateTexture;

		float m_Count = 0;


	};


}
