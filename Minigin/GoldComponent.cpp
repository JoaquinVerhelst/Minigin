#include "GoldComponent.h"

#include "SoundServiceLocator.h"
#include "World.h"
#include "ScoreComponent.h"
#include "Time.h"
#include "SimpleRenderComponent.h"
#include "SoundServiceLocator.h"
#include "GameObject.h"

dae::GoldComponent::GoldComponent(GameObject* owner, int gridindex)
	: TreasureComponent(owner)
	, m_CurrentGridIndex{ gridindex }
	, m_IsBroken{false}
	, m_IsFalling{false}
	, m_FallingDistance{ 0 }
	, m_PushDirection{ -1 }
	, m_IsPushed{false}
	, m_WaitToFallCounter{ 0 }
	, m_WaitToFallTime{ 2.f }
{

}

void dae::GoldComponent::Update()
{

	auto pos = GetOwner()->GetPosition().GetPosition();

	//float gravity = 9.381f;
	float fallingSpeed = 120.f;
	//get the gridindex dit in zit

	// kijk of de gridindex + 1 is gedigd

	auto grid = World::GetInstance().GetWorldGrid();

	if (grid[m_CurrentGridIndex + 1]->isCellBroken)
	{

		if (m_WaitToFallCounter >= m_WaitToFallTime)
		{
			++m_FallingDistance;
			m_IsFalling = true;
			++m_CurrentGridIndex;
		}
		else
		{
			m_WaitToFallCounter += Time::GetInstance().GetDeltaTime();
		}

	}
	if (m_IsFalling)
	{

		GetOwner()->SetPosition(pos.x, pos.y + fallingSpeed * Time::GetInstance().GetDeltaTime());

		if (pos.y >= grid[m_CurrentGridIndex]->position.y)
		{
			m_IsFalling = false;

			if (m_FallingDistance >= 2)
			{
				SoundServiceLocator::GetSoundSysyem().Play(2, 1.0f);
				GetOwner()->GetComponent<SimpleRenderComponent>().SetTexture("../Data/Sprites/GoldBroken.png");
				m_IsBroken = true;
			}

			m_FallingDistance = 0;
			m_WaitToFallCounter = 0;
		}
	}

	if (m_IsPushed)
	{
		CalculatePush();
	}




}

void dae::GoldComponent::Init()
{
}

bool dae::GoldComponent::PickUpTreasure(GameObject* actor)
{

	if (m_IsBroken)
	{
		SoundServiceLocator::GetSoundSysyem().Play(1, 1.0f);
		actor->GetComponent<ScoreComponent>().AddScore(500);
		return true;
	}

	return false;
}

bool dae::GoldComponent::CalculateCollision(GameObject* actor)
{
	auto pos = GetOwner()->GetPosition().GetPosition();

	auto playerPos = actor->GetPosition().GetPosition();



	if (!m_IsFalling)
	{
		if (pos.y > playerPos.y || pos.y < playerPos.y)
		{
			return true;
		}
		else if (pos.x > playerPos.x) {
			m_PushDirection = 0;
			m_CurrentGridIndex += 14;
			m_IsPushed = true;
			return false;
		}
		else if (pos.x < playerPos.x) {
			m_PushDirection = 1;
			m_CurrentGridIndex += 14;
			m_IsPushed = true;
			return false;
		}
	}
	else
	{
		//player dead
	}

	
	return false;


}

void dae::GoldComponent::CalculatePush()
{
	float pushSpeed = 100.f;
	auto pos = GetOwner()->GetPosition().GetPosition();
	auto grid = World::GetInstance().GetWorldGrid();



	if (m_PushDirection == 0)
	{
		GetOwner()->SetPosition(pos.x + pushSpeed * Time::GetInstance().GetDeltaTime() , pos.y);


		if (pos.x >= grid[m_CurrentGridIndex]->position.x)
		{
			m_IsPushed = false;
		}


	}
	else
	{
		GetOwner()->SetPosition(pos.x - pushSpeed * Time::GetInstance().GetDeltaTime(), pos.y);


		if (pos.x <= grid[m_CurrentGridIndex]->position.x)
		{
			m_IsPushed = false;
		}
	}



}
