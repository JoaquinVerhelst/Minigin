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


namespace dae
{

	NobbinComponent::NobbinComponent(GameObject* owner, float walkSpeed)
		: dae::CharacterComponent(owner, walkSpeed, false)
        , m_WalkDirection{0}
        , m_Counter{5.f}
        , m_UpdateDirectionTime{0.15f}
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

        if (m_CurrentCell->id + 1 <= gridSize && grid[m_CurrentCell->id + 1]->isCellBroken && m_WalkDirection != 1)
        {
            availableDirections.push_back(0); // down
        }
        if (m_CurrentCell->id - 1 >= 0 && grid[m_CurrentCell->id - 1]->isCellBroken && m_WalkDirection != 0)
        {
            availableDirections.push_back(1); // up
        }
        if (m_CurrentCell->id - 14 >= 0 && grid[m_CurrentCell->id - 14]->isCellBroken && m_WalkDirection != 3)
        {
            availableDirections.push_back(2); // left
        }
        if (m_CurrentCell->id + 14 <= gridSize && grid[m_CurrentCell->id + 14]->isCellBroken && m_WalkDirection != 2)
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


            for (size_t i = 0; i < availableDirections.size(); i++)
            {
                std::cout << " " << availableDirections[i];
            }
            std::cout << '\n';



            m_WalkDirection = randomDirection;



            switch (randomDirection)
            {
            case 0: // down
                SetState(new VerticalWalkState());
                SetDirection(0);
                break;
            case 1: // up
                SetState(new VerticalWalkState());
                SetDirection(1);
                break;
            case 2: // left
                SetState(new HorizontalWalkState());
                SetDirection(1);
                break;
            case 3: // right
                SetState(new HorizontalWalkState());
                SetDirection(0);
                break;
            }




        }
        // if the nobbin goes outside the grid or dead end -> return from where it came from 
        else if (availableDirections.empty())
        {
            switch (m_WalkDirection)
            {
            case 0: // down
                SetState(new VerticalWalkState());
                SetDirection(1);

                m_WalkDirection = 1;
                break;
            case 1: // up
                SetState(new VerticalWalkState());
                SetDirection(0);

                m_WalkDirection = 0;
                break;
            case 2: // left
                SetState(new HorizontalWalkState());
                SetDirection(0);

                m_WalkDirection = 3;
                break;
            case 3: // right

                SetState(new HorizontalWalkState());
                SetDirection(1);

                m_WalkDirection = 2;
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
            m_CurrentCell->temp = false;
            m_CurrentCell = cell;
            m_CurrentCell->temp = true;

            CalculateMovement();
        }
    }


	void NobbinComponent::Update()
	{

        if (!m_ControlledByPlayer)
        {
            m_Counter += Time::GetInstance().GetDeltaTime();

            m_CurrentState->CalculateDirection(GetOwner(), this);

            if (m_CurrentCell->isCellBroken)
            {
                m_CurrentState->Update(GetOwner(), this);
            }
            
        }
        else
        {
            glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();

            if (!World::GetInstance().GetOverlappedUnbrokenCell(currentPos, glm::vec2(50, 50)))
            {
                m_CurrentState->Update(GetOwner(), this);
            }
        }
	}


	void NobbinComponent::Init()
	{
        SetState(new HorizontalWalkState());
        SetDirection(1);

        glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();
        GridCell* cell = World::GetInstance().GetOverlappedCell(currentPos, glm::vec2(1, 1));
        m_CurrentCell = cell;
        m_CurrentCell->temp = true;

        CalculateMovement();
	}

	void NobbinComponent::Render() const
	{
	}



}