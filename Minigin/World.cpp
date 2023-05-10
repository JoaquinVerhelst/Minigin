#include "World.h"

#include "Renderer.h"
#include "Time.h"

using namespace dae;





World::~World()
{
    for (size_t i = 0; i < m_Grid.size(); i++)
    {
        delete m_Grid[i];
        m_Grid[i] = nullptr;
    }



}

void World::Init(int rows, int columns, SDL_Window* window)
{

    int width{};
    int height{};

    SDL_GetWindowSize(window, &width, &height);

    m_Rows = rows;
    m_Columns = columns;


    m_CellWidth = width / m_Rows;
    m_CellHeight = height / m_Columns;


    glm::vec2 pos = {};


    for (size_t r = 0; r < m_Rows; ++r)
    {
        for (size_t c = 0; c < m_Columns; ++c)
        {
            pos = { r * m_CellWidth, c * m_CellHeight };

            GridCell* cell = new GridCell();
            cell->isCellBroken = false;
            cell->position = pos;

            m_Grid.emplace_back(cell);
        }
    }

    m_UpdateTexture = true;

}

void World::Render() const
{
    if (m_UpdateTexture)
    {
        SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), Renderer::GetInstance().GetCanvas());


        SDL_Rect rect = {};


        for (size_t i = 0; i < m_Grid.size(); i++)
        {
            int x = static_cast<int>(m_Grid[i]->position.x);
            int y = static_cast<int>(m_Grid[i]->position.y);

            rect = { x , y, m_CellWidth, m_CellHeight };

            //if (m_Grid[i]->isCellBroken)
            //{
            //    SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 128, 0, 255);
            //    SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
            //}
 
            if (i % 3 == 0)
            {
                SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 204, 0, 255);
            }
            else if (i % 3 == 1)
            {
                SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 153, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 102, 0, 255);
            }

            SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
            

        }
    }



}

void dae::World::ResetGrid()
{





}

void dae::World::Update()
{

    if (m_UpdateTexture)
    {
        m_Count += Time::GetInstance().GetDeltaTime();

        if (m_Count >= 0.2f)
        {
            m_UpdateTexture = false;
        }
    }

}

bool World::IsOverlappingWithWorld(const glm::vec2& position, const glm::vec2& size) const
{
    for (const auto& cell : m_Grid)
    {
        if (!cell->isCellBroken)
        {
            const glm::vec2& cellPos = cell->position;

            if (position.x + size.x > cellPos.x && position.x < cellPos.x + m_CellWidth &&
                position.y + size.y > cellPos.y && position.y < cellPos.y + m_CellHeight)
            {

                return true;
            }
        }
    }

    return false;
}

GridCell* World::GetOverlappedCell(const glm::vec2& position, const glm::vec2& size) const
{
    for (const auto& cell : m_Grid)
    {
        if (!cell->isCellBroken)
        {
            const glm::vec2& cellPos = cell->position;

            if (position.x + size.x > cellPos.x && position.x < cellPos.x + m_CellWidth &&
                position.y + size.y > cellPos.y && position.y < cellPos.y + m_CellHeight)
            {
                return cell;
            }
        }
    }

    return nullptr;

}

void World::BreakWorld(const glm::vec2& position, const glm::vec2& size)
{
    GridCell* cell = GetOverlappedCell(position, size);


    if (cell)
    {
        cell->isCellBroken = true;
    }


}

glm::vec2 dae::World::GetCellSize()
{
    return glm::vec2(m_CellWidth, m_CellHeight);
}

void dae::World::UpdateWorld()
{
    m_UpdateTexture = true;
}
