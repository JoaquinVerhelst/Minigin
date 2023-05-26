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
	, m_PushDirection{ -1 }
	, m_WaitToFallCounter{ 0 }
	, m_WaitToFallTime{ 2.f }
	, m_CurrentState{ new IdleGoldState() }
	, m_PushSpeed{pushspeed}
	, m_FallSpeed{fallspeed}
{

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
	SoundServiceLocator::GetSoundSysyem().Play(1, 1.0f);
	actor->GetComponent<ScoreComponent>().AddScore(500);
	return true;
}

bool dae::GoldComponent::HandleCollision(GameObject* actor)
{
	return m_CurrentState->HandleCollision(actor, GetOwner());
}

void dae::GoldComponent::CalculateFall()
{
	float fallingSpeed = 120.f;
	auto pos = GetOwner()->GetPosition().GetPosition();
	auto grid = World::GetInstance().GetWorldGrid();

	GetOwner()->SetPosition(pos.x, pos.y + fallingSpeed * Time::GetInstance().GetDeltaTime());

	if (pos.y >= grid[m_CurrentGridIndex]->position.y)
	{
		GetOwner()->SetPosition(std::round(pos.x), std::round(pos.y + fallingSpeed * Time::GetInstance().GetDeltaTime()));

		if (m_FallingDistance >= 2)
		{
			SoundServiceLocator::GetSoundSysyem().Play(2, 1.0f);
			GetOwner()->GetComponent<SimpleRenderComponent>().SetTexture("../Data/Sprites/GoldBroken.png");

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
	m_PushDirection = 0;
	m_CurrentGridIndex += 14;


	if (m_CurrentGridIndex >= grid.size())
	{
		m_CurrentGridIndex -= 14;
		return;
	}


	SetState(new PushedGoldState());

}

void dae::GoldComponent::PushLeft()
{
	m_PushDirection = 1;
	m_CurrentGridIndex -= 14;

	if (m_CurrentGridIndex <= 0)
	{
		m_CurrentGridIndex += 14;
		return;
	}

	SetState(new PushedGoldState());

}

void dae::GoldComponent::SetState(dae::GoldState* newState)
{
	if (m_CurrentState != nullptr)
	{
		delete m_CurrentState;
		m_CurrentState = nullptr;
	}

	std::cout << "new state" << newState <<  " " << int(newState->GetType() )<< '\n';

	m_CurrentState = newState;
}