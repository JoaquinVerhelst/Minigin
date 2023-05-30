#include "GoldStateMachine.h"
#include "GoldComponent.h"
#include "World.h"
#include "Time.h"
#include "CharacterComponent.h"


namespace dae
{



	//-----------IDLE---------------------------------------------------------------------------
	/////////////////////////////////////////////////////////////////////////////////////////////


	bool IdleGoldState::HandleCollision(GameObject* player, GameObject* gold)
	{



		auto goldPos = gold->GetPosition().GetPosition();
		auto playerPos = player->GetPosition().GetPosition();

		if (goldPos.y == playerPos.y)
		{
			if (goldPos.x > playerPos.x)
			{
				gold->GetComponent<GoldComponent>().PushRight();
			}
			else
			{
				gold->GetComponent<GoldComponent>().PushLeft();
			}
		}

		return false;
	}

	void IdleGoldState::Update(GameObject* , GoldComponent* goldComp)
	{
		goldComp->CheckForFalling();
	}

	
	//-----------BROKEN--------------------------------------------------------------------------
	/////////////////////////////////////////////////////////////////////////////////////////////

	bool BrokenGoldState::HandleCollision(GameObject* actor, GameObject* gold)
	{
		gold->GetComponent<GoldComponent>().PickUpTreasure(actor);
		return true;

	}

	void BrokenGoldState::Update(GameObject* , GoldComponent* )
	{
	}


	//-----------PUSHED--------------------------------------------------------------------------
	/////////////////////////////////////////////////////////////////////////////////////////////


	bool PushedGoldState::HandleCollision(GameObject* , GameObject* )
	{
		return false;
	}

	void PushedGoldState::Update(GameObject* goldActor, GoldComponent* goldComp)
	{
		auto pos = goldActor->GetPosition().GetPosition();
		auto grid = World::GetInstance().GetWorldGrid();

		float pushSpeed = goldComp->GetPushSpeed();


		if (goldComp->GetPushDirection() == 0)
		{
			goldActor->SetPosition(pos.x + pushSpeed * Time::GetInstance().GetDeltaTime(), pos.y);

			if (pos.x >= grid[goldComp->GetCurrentGridIndex()]->position.x)
			{
				goldComp->SetState(new IdleGoldState());
			}

		}
		else
		{
			goldActor->SetPosition(pos.x - pushSpeed * Time::GetInstance().GetDeltaTime(), pos.y);

			if (pos.x <= grid[goldComp->GetCurrentGridIndex()]->position.x)
			{
				goldComp->SetState(new IdleGoldState());
			}
		}
	}

	//-----------FALLING--------------------------------------------------------------------------
	//////////////////////////////////////////////////////////////////////////////////////////////


	bool FallingGoldState::HandleCollision(GameObject* playerActor, GameObject* )
	{
		playerActor->GetComponent<CharacterComponent>().SetState(new DeadState());
		return false;
	}

	void FallingGoldState::Update(GameObject* , GoldComponent* goldComp)
	{
		goldComp->CheckForFalling();
		goldComp->CalculateFall();
	}

}
