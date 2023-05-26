#include "World.h"

#include "Renderer.h"
#include "Time.h"
#include "ScoreComponent.h"
#include "SoundServiceLocator.h"
#include "TreasureComponent.h"
#include <iostream>
#include "SimpleRenderComponent.h"
#include "CharacterComponent.h"


using namespace dae;





World::~World()
{
    for (size_t i = 0; i < m_Grid.size(); i++)
    {
        delete m_Grid[i];
        m_Grid[i] = nullptr;
    }


    for (size_t i = 0; i < m_Levels.size(); i++)
    {
        delete m_Levels[i];
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



    glm::ivec2 pos = {};


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


   // std::vector<int> index = { 1,2,3,6,10,12,11,13,15,16,20 };




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


            if (m_Grid[i]->isCellBroken)
            {
                SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 0, 255);
                SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
            }
            

        }
    }



   // RenderBrokenPath();
}

void dae::World::Reset()
{
    for (size_t i = 0; i < m_Grid.size(); i++)
    {
        m_Grid[i]->isCellBroken = false;
    }




}

void dae::World::Update()
{

    if (m_UpdateTexture)
    {
        m_Count += Time::GetInstance().GetDeltaTime();

        if (m_Count >= 0.5f)
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

size_t dae::World::GetOverlappedTreasureIndex(const glm::vec2& position, const glm::vec2& size) const
{
    for (size_t i = 0; i < m_Treasure.size(); i++)
    {
        const glm::vec2& treasurePos = m_Treasure[i]->GetPosition().GetPosition();
        const glm::vec2& treasureShape = m_Treasure[i]->GetShape();

        if (position.x + size.x > treasurePos.x && position.x < treasurePos.x + treasureShape.x &&
            position.y + size.y > treasurePos.y && position.y < treasurePos.y + treasureShape.y)
        {
            return i;
        }
        
    }

    return static_cast<size_t>(-1);
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

void World::BreakWorld(GameObject* actor, const glm::vec2& size)
{

    auto pos = actor->GetPosition().GetPosition();


    GridCell* cell = GetOverlappedCell(pos, size);

    
    if (cell && cell->isCellBroken == false)
    {
        cell->isCellBroken = true;
    }


}

bool dae::World::CheckForTreasure(GameObject* actor, const glm::vec2& size)
{
    auto pos = actor->GetPosition().GetPosition();

    size_t treasureIndex = GetOverlappedTreasureIndex(pos, size);

    if(treasureIndex != -1)
    {
        if (m_Treasure[treasureIndex]->GetComponent<TreasureComponent>().HandleCollision(actor))
        {
            m_Treasure[treasureIndex]->Destroy();
            m_Treasure.erase(m_Treasure.begin() + treasureIndex);

            return false;

        }
        else
        {
            return true;
        }
    }

    return false;
}

glm::vec2 dae::World::GetCellSize()
{
    return glm::vec2(m_CellWidth, m_CellHeight);
}

void dae::World::UpdateWorld()
{
    m_UpdateTexture = true;
}

std::vector<GridCell*> dae::World::GetWorldGrid()
{
    return m_Grid;
}

void dae::World::PlaceTreasure(std::shared_ptr<GameObject> treasure, int gridIndex)
{
    treasure->SetPosition(m_Grid[gridIndex]->position.x, m_Grid[gridIndex]->position.y);
    m_Treasure.emplace_back(treasure);
}

void dae::World::PlaceGameObject(std::shared_ptr<GameObject> gameobject, int gridIndex)
{
    gameobject->SetPosition(m_Grid[gridIndex]->position.x, m_Grid[gridIndex]->position.y);
}

void dae::World::BreakGridIndexes(std::vector<int> gridIndexes)
{
    for (size_t i = 0; i < gridIndexes.size(); i++)
    {
        if (gridIndexes[i] >= m_Grid.size())
            break;

        m_Grid[gridIndexes[i]]->isCellBroken = true;
    }
}

void dae::World::SetGameMode(GameModeTypes gameMode)
{
    m_CurrentGameMode = gameMode;
}




void dae::World::ResetAndLoadWorld(int index)
{
    Reset();

    m_CurrentWorldIndex = index;

    Renderer::GetInstance().SetBackgroundTexture(m_Levels[index]->background->GetComponent<SimpleRenderComponent>().GetTexture()->GetSDLTexture());


    if (index != 0 && index != 4)
    {
        m_Levels[index]->player1->GetComponent<CharacterComponent>().SetState(new IdleState());
    }
  
    

    // break the world




    // place players

    // place treasure

    // place enemy Manager

}

void dae::World::AddLevelInfo(LevelInfo* levelinfo)
{
    m_Levels.emplace_back(levelinfo);
}

