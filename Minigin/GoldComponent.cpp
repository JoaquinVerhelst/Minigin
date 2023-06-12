#include "GoldComponent.h"

#include "SoundServiceLocator.h"
#include "World.h"
#include "ScoreComponent.h"
#include "Time.h"
#include "SimpleRenderComponent.h"
#include "SoundServiceLocator.h"
#include "GameObject.h"

#include <iostream>


dae::GoldComponent::GoldComponent(GameObject* owner, int gridindex, float pushspeed, float fallspeed)
	: TreasureComponent(owner)
	, m_CurrentGridIndex{ gridindex }
	, m_FallingDistance{ 0 }
	, m_WaitToFallCounter{ 0 }
	, m_WaitToFallTime{ 2.f }
	, m_CurrentState{ new IdleGoldState() }
	, m_PushSpeed{pushspeed}
	, m_FallSpeed{fallspeed}
	, m_Columns{}
	, m_BrokenSprite{""}
{
	m_Columns = World::GetInstance().GetColumns();
}

dae::GoldComponent::~GoldComponent()
{
	if (m_CurrentState != nullptr)
	{
		delete m_CurrentState;
		m_CurrentState = nullptr;
	}
}

void dae::GoldComponent::Update()
{
	m_CurrentState->Update(GetOwner(), this);
}

void dae::GoldComponent::FixedUpdate()
{
}

void dae::GoldComponent::Init()
{
}

bool dae::GoldComponent::PickUpTreasure(GameObject* actor)
{
	SoundServiceLocator::GetSoundSystem().Play(1, 1.0f);
	actor->GetComponent<ScoreComponent>().AddScore(500);
	return true;
}

bool dae::GoldComponent::HandleCollision(GameObject* actor)
{
	return m_CurrentState->HandleCollision(actor, GetOwner());
}

void dae::GoldComponent::CalculateFall()
{

	auto pos = GetOwner()->GetPosition().GetPosition();
	auto grid = World::GetInstance().GetWorldGrid();

	GetOwner()->SetPosition(pos.x, pos.y + m_FallSpeed * Time::GetInstance().GetDeltaTime());

	if (pos.y >= grid[m_CurrentGridIndex]->position.y)
	{
		GetOwner()->SetPosition(std::round(pos.x), std::round(pos.y + m_FallSpeed * Time::GetInstance().GetDeltaTime()));

		// falling 2 stories
		if (m_FallingDistance >= 2)
		{
			// playing sound 2
			SoundServiceLocator::GetSoundSystem().Play(2, 1.0f);
			GetOwner()->GetComponent<SimpleRenderComponent>().SetTexture(m_BrokenSprite);

			SetState(new BrokenGoldState());
		}
		else
		{
			SetState(new IdleGoldState());
		}

		m_FallingDistance = 0;
		m_WaitToFallCounter = 0;
	}
}

void dae::GoldComponent::CheckForFalling()
{
	auto pos = GetOwner()->GetPosition().GetPosition();
	auto grid = World::GetInstance().GetWorldGrid();

	if (grid[m_CurrentGridIndex + 1]->isCellBroken)
	{
		if (m_WaitToFallCounter >= m_WaitToFallTime)
		{
			++m_FallingDistance;
			++m_CurrentGridIndex;

			if (m_CurrentState->GetType() != GoldStateType::Falling)
			{
				SetState(new FallingGoldState());
			}
		}
		else
		{
			m_WaitToFallCounter += Time::GetInstance().GetDeltaTime();
		}
	}
}

void dae::GoldComponent::PushRight()
{
	auto grid = World::GetInstance().GetWorldGrid();
	m_CurrentGridIndex += m_Columns;


	if (m_CurrentGridIndex >= static_cast<int>(grid.size()))
	{
		m_CurrentGridIndex -= m_Columns;
		return;
	}


	SetState(new PushedRightGoldState());

}

void dae::GoldComponent::PushLeft()
{
	m_CurrentGridIndex -= m_Columns;

	if (m_CurrentGridIndex <= 0)
	{
		m_CurrentGridIndex += m_Columns;
		return;
	}

	SetState(new PushedLeftGoldState());

}

void dae::GoldComponent::SetState(dae::GoldState* newState)
{
	if (m_CurrentState != nullptr)
	{
		delete m_CurrentState;
		m_CurrentState = nullptr;
	}

	m_CurrentState = newState;
}