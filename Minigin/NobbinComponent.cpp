#include "NobbinComponent.h"

#include "Time.h"
#include <glm/glm.hpp>
#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "World.h"
#include <iostream>
#include <random>
#include "SimpleRenderComponent.h"

namespace dae
{

	NobbinComponent::NobbinComponent(GameObject* owner, float walkSpeed, bool isControlledByPlayer)
		: dae::CharacterComponent(owner, walkSpeed, isControlledByPlayer)
        , m_Counter{5.f}
        , m_UpdateDirectionTime{0.15f}
        , m_RageCounter{ 0.f }
        , m_RageTime{ 9.f }
        , m_StartRage{false}
        , m_CoolDownTime{ 9.f }
        , m_CanRage{ true}
	{



	}

	NobbinComponent::~NobbinComponent()
	{
	}




    void NobbinComponent::CalculateMovement()
	{
        auto grid = World::GetInstance().GetWorldGrid();
        const int gridSize = static_cast<int>(grid.size());

        std::vector<int> availableDirections;

        if (m_CurrentCell->id + 1 <= gridSize - 1 && grid[m_CurrentCell->id + 1]->isCellBroken && m_Direction != Direction::Up)
        {
            availableDirections.push_back(0); // down
        }
        if (m_CurrentCell->id - 1 >= 0 && grid[m_CurrentCell->id - 1]->isCellBroken && m_Direction != Direction::Down)
        {
            availableDirections.push_back(1); // up
        }
        if (m_CurrentCell->id - 14 >= 0 && grid[m_CurrentCell->id - 14]->isCellBroken && m_Direction != Direction::Right)
        {
            availableDirections.push_back(2); // left
        }
        if (m_CurrentCell->id + 14 <= gridSize - 1 && grid[m_CurrentCell->id + 14]->isCellBroken && m_Direction != Direction::Left)
        {
            availableDirections.push_back(3); // right
        }



        // Randomly select a direction among the available options
        if (!availableDirections.empty() && m_Counter >= m_UpdateDirectionTime)
        {
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> dist(0, static_cast<int>(availableDirections.size() - 1));
            int randomDirection = availableDirections[dist(rng)];
            m_Counter = 0;


            //for (size_t i = 0; i < availableDirections.size(); i++)
            //{
            //    std::cout << " " << availableDirections[i];
            //}
            //std::cout << '\n';




            switch (randomDirection)
            {
            case 0: // down
                SetState(new VerticalWalkState());
                SetDirection(Direction::Down);
                break;
            case 1: // up
                SetState(new VerticalWalkState());
                SetDirection(Direction::Up);
                break;
            case 2: // left
                SetState(new HorizontalWalkState());
                SetDirection(Direction::Left);
                break;
            case 3: // right
                SetState(new HorizontalWalkState());
                SetDirection(Direction::Right);
                break;
            }




        }
        // if the nobbin goes outside the grid or dead end -> return from where it came from 
        else if (availableDirections.empty())
        {
            switch (m_Direction)
            {
            case Direction::Down: // down
                SetState(new VerticalWalkState());
                SetDirection(Direction::Up);
                break;
            case Direction::Up: // up
                SetState(new VerticalWalkState());
                SetDirection(Direction::Down);
                break;
            case Direction::Left: // left
                SetState(new HorizontalWalkState());
                SetDirection(Direction::Right);

                break;
            case Direction::Right: // right

                SetState(new HorizontalWalkState());
                SetDirection(Direction::Left);
                break;
            }
        }



	}

    void NobbinComponent::CalculateCell()
    {
        glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();

        GridCell* cell = World::GetInstance().GetOverlappedCell(currentPos, glm::vec2(1, 1));


        if (cell && cell != m_CurrentCell && cell->isCellBroken)
        {
            m_CurrentCell = cell;
            CalculateMovement();
        }
    }


	void NobbinComponent::Update()
	{
        
        float deltaTime = Time::GetInstance().GetDeltaTime();


        if (!m_ControlledByPlayer)
        {
            m_Counter += deltaTime;

            m_CurrentState->CalculateDirection(GetOwner(), this);

            if (m_CurrentCell->isCellBroken)
            {
                m_CurrentState->Update(GetOwner(), this);
            }
            
        }
        else
        {
            glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();



            if (!m_StartRage && CalculateBounceBack())
            {
                m_CurrentState->Update(GetOwner(), this);
            }
            else if (m_StartRage)
            {
                m_CurrentState->Update(GetOwner(), this);
            }
        }



        if (m_StartRage)
        {
            World::GetInstance().BreakWorld(GetOwner(), m_CellSize);


            m_RageCounter += deltaTime;

            if (m_RageCounter >= m_RageTime)
            {
                m_RageCounter = 0;
                m_StartRage = false;
                m_CanRage = false;
                GetOwner()->GetComponent<SimpleRenderComponent>().SetTexture(m_Sprites.enemySprite);
            }
        }
        else if (!m_CanRage)
        {
            m_RageCounter += deltaTime;

            if (m_RageCounter >= m_CoolDownTime)
            {
                m_RageCounter = 0;
                m_CanRage = true;
            }
        }
        else
        {

        }

	}


	void NobbinComponent::Init()
	{
        SetState(new HorizontalWalkState());
        SetDirection(Direction::Left);

        glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();
        GridCell* cell = World::GetInstance().GetOverlappedCell(currentPos, glm::vec2(1, 1));
        m_CurrentCell = cell;

        CalculateMovement();
	}

	void NobbinComponent::Render() const
	{
        if (m_StartRage)
        {
            CharacterComponent::Render();
        }
	}


    void NobbinComponent::UseSpecialty()
    {
        if (!m_StartRage && m_CanRage)
        {
            m_StartRage = true;
            GetOwner()->GetComponent<SimpleRenderComponent>().SetTexture(m_Sprites.rageSprite);
        }
    }

    bool NobbinComponent::CalculateBounceBack()
    {

        glm::vec3 currentPos = GetPosition();


        auto grid = World::GetInstance().GetWorldGrid();
        const int gridSize = static_cast<int>(grid.size());
        glm::vec2 size(1.f, 1.f);
        float offset = 2.f;

        if (m_Direction == Direction::Up)
        {
            m_CurrentCell = World::GetInstance().GetOverlappedCell(glm::vec2(currentPos.x + m_CellSize.x / 2, currentPos.y + m_CellSize.y - offset), size);

            if (m_CurrentCell && m_CurrentCell->id - 1 >= 0 && grid[m_CurrentCell->id - 1]->isCellBroken)
            {
                return true;
            }
        }
        if (m_Direction == Direction::Down)
        {
            
            m_CurrentCell = World::GetInstance().GetOverlappedCell(glm::vec2(currentPos.x + m_CellSize.x / 2, currentPos.y ), size);

            if (m_CurrentCell && m_CurrentCell->id + 1 <= gridSize - 1 && grid[m_CurrentCell->id + 1]->isCellBroken)
            {
                return true;
            }
            
        }
        if (m_Direction == Direction::Left)
        {
            m_CurrentCell = World::GetInstance().GetOverlappedCell(glm::vec2(currentPos.x + m_CellSize.x - offset, currentPos.y + m_CellSize.x / 2), size);
            if (m_CurrentCell && m_CurrentCell->id - 14 >= 0 && grid[m_CurrentCell->id - 14]->isCellBroken)
            {
                return true;
            }
        }
        if (m_Direction == Direction::Right)
        {
            m_CurrentCell = World::GetInstance().GetOverlappedCell(glm::vec2(currentPos.x, currentPos.y + m_CellSize.x / 2), size);

            if (m_CurrentCell && m_CurrentCell->id + 14 <= gridSize - 1 && grid[m_CurrentCell->id + 14]->isCellBroken)
            {
                return true;
            }
        }
        

        return false;
    }

}