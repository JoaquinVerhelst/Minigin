#include "World.h"

#include "Renderer.h"
#include "Time.h"
#include "ScoreComponent.h"
#include "SoundServiceLocator.h"
#include "TreasureComponent.h"
#include <iostream>
#include "SimpleRenderComponent.h"
#include "CharacterComponent.h"

#include "EmeraldComponent.h"
#include "GoldComponent.h"
#include "TreasureComponent.h"
#include "Scene.h"
#include "JsonManager.h"

#include "InputManager.h"

using namespace dae;





World::~World()
{
    for (size_t i = 0; i < m_Grid.size(); i++)
    {
        delete m_Grid[i];
        m_Grid[i] = nullptr;
    }


    //for (size_t i = 0; i < m_Levels.size(); i++)
    //{
    //    delete m_Levels[i];
    //}


}

void World::Init(int rows, int columns, SDL_Window* window)
{

    int width{};
    int height{};

    SDL_GetWindowSize(window, &width, &height);

    m_Rows = rows;
    m_Columns = columns;

    m_GameOver = false;
    m_PlayersDead = 0;

    m_CellWidth = width / m_Rows;
    m_CellHeight = height / m_Columns;

    m_NobbinPlayer = nullptr;

    glm::ivec2 pos = {};
    int index = 0;

    for (size_t r = 0; r < m_Rows; ++r)
    {
        for (size_t c = 0; c < m_Columns; ++c)
        {
            pos = { r * m_CellWidth, c * m_CellHeight };

            GridCell* cell = new GridCell();
            cell->id = index;
            cell->isCellBroken = false;
            cell->position = pos;


            cell->temp = false;

            ++index;
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
            if (m_Grid[i]->isCellBroken)
            {
                int x = static_cast<int>(m_Grid[i]->position.x);
                int y = static_cast<int>(m_Grid[i]->position.y);

                rect = { x , y, m_CellWidth, m_CellHeight };
                SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 0, 255);
                SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
            }
         
            if (m_Grid[i]->temp)
            {
                int x = static_cast<int>(m_Grid[i]->position.x);
                int y = static_cast<int>(m_Grid[i]->position.y);

                rect = { x , y, m_CellWidth, m_CellHeight };
                SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
                SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
            }

        }
    }

    SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), nullptr);
    for (size_t i = 0; i < m_Grid.size(); i++)
    {
        if (m_Grid[i]->temp)
        {
            int x = static_cast<int>(m_Grid[i]->position.x);
            int y = static_cast<int>(m_Grid[i]->position.y);
            SDL_Rect rect = { x , y, m_CellWidth, m_CellHeight };
            SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
            SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
        }

    }
}

void dae::World::Reset()
{





}

void dae::World::Update()
{

    if (m_UpdateTexture)
    {
        m_Count += Time::GetInstance().GetDeltaTime();

        if (m_Count >= .2f)
        {
            m_UpdateTexture = false;
            m_Count = 0;
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

GridCell* dae::World::GetOverlappedUnbrokenCell(const glm::vec2& position, const glm::vec2& size) const
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

GridCell* World::GetOverlappedCell(const glm::vec2& position, const glm::vec2& size) const
{
    for (const auto& cell : m_Grid)
    {
        const glm::vec2& cellPos = cell->position;

        if (position.x + size.x > cellPos.x && position.x < cellPos.x + m_CellWidth &&
            position.y + size.y > cellPos.y && position.y < cellPos.y + m_CellHeight)
        {
            return cell;
        }
        
    }

    return nullptr;

}

void World::BreakWorld(GameObject* actor, const glm::vec2& size)
{
    auto pos = actor->GetPosition().GetPosition();

    GridCell* cell = GetOverlappedUnbrokenCell(pos, size);
    
    if (cell && cell->isCellBroken == false)
        cell->isCellBroken = true;
    
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
  
         return true;
        
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
    if (gridIndex >= 0 && gridIndex < m_Grid.size())
    {
        treasure->SetPosition(m_Grid[gridIndex]->position.x, m_Grid[gridIndex]->position.y);
        m_Treasure.emplace_back(treasure);
    }

}

void dae::World::PlaceGameObject(std::shared_ptr<GameObject> gameobject, int gridIndex)
{
    gameobject->SetPosition(m_Grid[gridIndex]->position.x, m_Grid[gridIndex]->position.y);
}

void dae::World::PlaceGameObject(GameObject* gameobject, int gridIndex)
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


    m_CurrentWorldIndex = index;


    ResetLevel();


    LevelInfo* currentLevelInfo = SceneManager::GetInstance().GetSceneLevelInfo(m_CurrentWorldIndex);
    //auto treasures = SceneManager::GetInstance().GetSceneTreasure(m_CurrentWorldIndex);
    // 
    // background
    Renderer::GetInstance().SetBackgroundTexture(currentLevelInfo->background->GetComponent<SimpleRenderComponent>().GetTexture()->GetSDLTexture());

    // break the world
    BreakGridIndexes(currentLevelInfo->brokenWorldIndexs);



    //Place players and set idle
    if (currentLevelInfo->worldType == WorldTypes::Level)
    {
        for (size_t i = 0; i < m_Players.size(); i++)
        {
            m_Players[i]->GetComponent<CharacterComponent>().SetState(new IdleState());

            if (i == 0)
            {
                PlaceGameObject(m_Players[i], currentLevelInfo->player1PosIndex);
            }
            else
            {
                PlaceGameObject(m_Players[i], currentLevelInfo->player2PosIndex);
            }

        }
    }
  
    // place treasure

    auto go = std::make_shared<dae::GameObject>();

    for (int i = 0; i < currentLevelInfo->emeraldIndexs.size(); ++i)
    {
        go = std::make_shared<dae::GameObject>();
        go->AddComponent<SimpleRenderComponent>("../Data/Sprites/Emerald.png");
        go->AddComponent<EmeraldComponent>();

        SceneManager::GetInstance().GetScene(m_CurrentWorldIndex)->AddTreasure(go);

        World::GetInstance().PlaceTreasure(go, currentLevelInfo->emeraldIndexs[i]);
    }


    for (int i = 0; i < currentLevelInfo->goldIndexs.size(); ++i)
    {
        go = std::make_shared<dae::GameObject>();
        go->AddComponent<SimpleRenderComponent>("../Data/Sprites/Gold.png");
        go->AddComponent<GoldComponent>(currentLevelInfo->goldIndexs[i] );

        SceneManager::GetInstance().GetScene(m_CurrentWorldIndex)->AddTreasure(go);

        World::GetInstance().PlaceTreasure(go, currentLevelInfo->goldIndexs[i] );
    }

    // place enemy Manager





    //TODO : If all players are dead -> end
    // TODO : If all emeralds are gone -> Next level
    // todo If all enemy dead -> next level





    UpdateWorld();


}


void dae::World::ResetGameMode()
{
    for (size_t i = 0; i < m_Players.size(); i++)
    {
        m_Players[i]->Destroy();
    }
    m_Players.clear();
    m_NobbinPlayer = nullptr;
    InputManager::GetInstance().ResetInput();
}

void dae::World::ResetLevel()
{
    for (size_t i = 0; i < m_Grid.size(); i++)
    {
        m_Grid[i]->isCellBroken = false;
    }

    SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), Renderer::GetInstance().GetCanvas());
    SDL_RenderClear(Renderer::GetInstance().GetSDLRenderer());

    m_Treasure.clear();
    SceneManager::GetInstance().GetScene(m_CurrentWorldIndex)->RemoveTreasures();
}


void dae::World::PlayerDied()
{
    ++m_PlayersDead;

    if (m_PlayersDead == m_Players.size())
    {
        if (m_CurrentGameMode == GameModeTypes::SinglePlayer)
        {
            m_GameOver = true;
            SceneManager::GetInstance().NextSceneByIndex(4);
            m_PlayersDead = 0;
            return;
        }
        if (m_CurrentGameMode == GameModeTypes::Coop)
        {
            m_GameOver = true;
            SceneManager::GetInstance().NextSceneByIndex(4);
            m_PlayersDead = 0;
            return;
        }
        else
        {
            ResetAndLoadWorld(0);
            m_PlayersDead = 0;
            return;
        }
    }
}

void dae::World::LoadSinglePlayer()
{

    ResetGameMode();


    auto player = JsonManager::GetInstance().LoadPlayer(0);
    m_Players.emplace_back(player);


    auto scenes = SceneManager::GetInstance().GetScenes();

    for (auto& scene : scenes)
    {
        if (scene->GetLevelInfo()->worldType == WorldTypes::Level)
        {
            scene->Add(player);
        }
    }
}

void dae::World::LoadCoop()
{
    ResetGameMode();


    auto player1 = JsonManager::GetInstance().LoadPlayer(0);
    m_Players.emplace_back(player1);

    auto player2 = JsonManager::GetInstance().LoadPlayer(1);
    m_Players.emplace_back(player2);


    auto scenes = SceneManager::GetInstance().GetScenes();

    for (auto& scene : scenes)
    {
        if (scene->GetLevelInfo()->worldType == WorldTypes::Level)
        {
            scene->Add(player1);
            scene->Add(player2);
        }
    }
}

void dae::World::LoadVersus()
{
    ResetGameMode();

    auto player1 = JsonManager::GetInstance().LoadPlayer(0);
    m_Players.emplace_back(player1);

    auto player2 = JsonManager::GetInstance().LoadNobbinPlayer();
    m_NobbinPlayer = player2;


    auto scenes = SceneManager::GetInstance().GetScenes();

    for (auto& scene : scenes)
    {
        if (scene->GetLevelInfo()->worldType == WorldTypes::Level)
        {
            scene->Add(player1);
            scene->Add(player2);
        }
    }

}

