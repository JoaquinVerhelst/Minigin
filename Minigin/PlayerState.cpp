#include "PlayerState.h"
#include <iostream>
#include "CharacterComponent.h"
#include "GameObject.h"
#include "World.h"

//-----------HORIZINTAL WALK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

dae::PlayerState* dae::HorizontalWalkState::HandleInput(Command::InputType inputType, PlayerStateType)
{
	if (inputType == Command::InputType::Up)
	{
		return new IdleState();
	}
	else
	{
		return nullptr;
	}
}

void dae::HorizontalWalkState::Update(GameObject* actor, CharacterComponent* character)
{


	glm::vec3 currentPos = character->GetPosition();

	if (World::GetInstance().CheckForTreasure(actor, { 50,50 }))
	{

		if (character->GetDirection() == 0)
		{
			actor->SetPosition(currentPos.x - .5f, currentPos.y);
		}
		else
		{
			actor->SetPosition(currentPos.x + .5f, currentPos.y);
		}
	}
	else
	{
		auto vec = character->CalculateWalk(character->GetDirection(), character->GetCellSize().y, currentPos.y, currentPos.x);


		actor->SetPosition(vec.y, vec.x);
	}

}


//-----------VERTICAL WALK-------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

dae::PlayerState* dae::VerticalWalkState::HandleInput(Command::InputType inputType, PlayerStateType)
{

	if (inputType == Command::InputType::Up)
	{
		return new IdleState();
	}
	else
	{
		return nullptr;
	}

}

void dae::VerticalWalkState::Update(GameObject* actor, CharacterComponent* character)
{

	glm::vec3 currentPos = character->GetPosition();


	if (World::GetInstance().CheckForTreasure(actor, { 50,50 }))
	{
		if (character->GetDirection() == 0)
		{
			actor->SetPosition(currentPos.x, currentPos.y - 0.5f);
		}
		else
		{
			actor->SetPosition(currentPos.x, currentPos.y + 0.5f);
		}
	}
	else
	{
		auto vec = character->CalculateWalk(character->GetDirection(), character->GetCellSize().x, currentPos.x, currentPos.y);

		actor->SetPosition(vec.x, vec.y);
	}

}



//-----------IDLE --------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////
dae::PlayerState* dae::IdleState::HandleInput(Command::InputType , PlayerStateType newStateType)
{
	if (newStateType == PlayerStateType::HorizontalWalk)
	{
		return new HorizontalWalkState();
	}
	else if (newStateType == PlayerStateType::VerticalWalk)	
	{
		return new VerticalWalkState();
	}
	else
	{
		return nullptr;
	}
}


