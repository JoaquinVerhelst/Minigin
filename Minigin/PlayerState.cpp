#include "PlayerState.h"
#include <iostream>
#include "CharacterComponent.h"
#include "SimpleRenderComponent.h"
#include "GameObject.h"
#include "World.h"

//-----------HORIZINTAL WALK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

void dae::HorizontalWalkState::UpdateSprite(GameObject* actor, int direction)
{

	SimpleRenderComponent& simpleRender = actor->GetComponent<SimpleRenderComponent>();

	if (direction == 0)
	{
		simpleRender.SetAngleAndFlip(0.f, SDL_FLIP_NONE);
	}
	else
	{
		simpleRender.SetAngleAndFlip(180.f, SDL_FLIP_VERTICAL);
	}
}

dae::PlayerState* dae::HorizontalWalkState::HandleInput(Command::InputType inputType, PlayerStateType newStateType)
{
	if (inputType == Command::InputType::Up)
	{
		return new IdleState();
	}
	else if (newStateType == PlayerStateType::VerticalWalk && inputType == Command::InputType::Pressed)
	{
		return new VerticalWalkState();
	}
	else
	{

		return nullptr;
	}
}

void dae::HorizontalWalkState::Update(GameObject* actor, CharacterComponent* character)
{

	glm::vec3 currentPos = character->GetPosition();

	auto vec = character->CalculateWalk(character->GetDirection(), character->GetCellSize().y, currentPos.y, currentPos.x);
	actor->SetPosition(vec.y, vec.x);

}

bool dae::HorizontalWalkState::CheckCollision(GameObject* actor, CharacterComponent* character)
{
	auto cellSize = character->GetCellSize();
	glm::vec3 currentPos = character->GetPosition();

	
	if (World::GetInstance().CheckForTreasure(actor, { cellSize.x - 1.f ,cellSize.y - 1.f }))
	{

		if (character->GetDirection() == 0)
		{
			actor->SetPosition(currentPos.x - .5f, currentPos.y);
		}
		else
		{
			actor->SetPosition(currentPos.x + .5f, currentPos.y);
		}
		return true;
	}

	return false;
}

void dae::HorizontalWalkState::CalculateDirection(GameObject* actor, CharacterComponent* character)
{
	glm::vec3 currentPos = actor->GetPosition().GetPosition();

	auto cell = character->GetCurrentCell();

	if (character->GetDirection() == 0)
	{
		if (currentPos.x >= cell->position.x )
		{
			character->CalculateCell();
		}
	}
	else
	{
		if (currentPos.x + 50 <= cell->position.x)
		{
			character->CalculateCell();
		}
	}
}


//-----------VERTICAL WALK-------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

void dae::VerticalWalkState::UpdateSprite(GameObject* actor, int direction)
{

	SimpleRenderComponent& simpleRender = actor->GetComponent<SimpleRenderComponent>();

	if (direction == 0)
	{
		simpleRender.SetAngleAndFlip(90.f, SDL_FLIP_NONE);
	}
	else
	{
		simpleRender.SetAngleAndFlip(270.f, SDL_FLIP_VERTICAL);
	}
}

dae::PlayerState* dae::VerticalWalkState::HandleInput(Command::InputType inputType, PlayerStateType newStateType)
{

	if (inputType == Command::InputType::Up)
	{
		return new IdleState();
	}
	else if (newStateType == PlayerStateType::HorizontalWalk && inputType == Command::InputType::Pressed)
	{
		return new HorizontalWalkState();
	}
	else
	{
		return nullptr;
	}

}

void dae::VerticalWalkState::Update(GameObject* actor, CharacterComponent* character)
{
	glm::vec3 currentPos = character->GetPosition();

	auto vec = character->CalculateWalk(character->GetDirection(), character->GetCellSize().x, currentPos.x, currentPos.y);
	actor->SetPosition(vec.x, vec.y);
}

bool dae::VerticalWalkState::CheckCollision(GameObject* actor, CharacterComponent* character)
{
	auto cellSize = character->GetCellSize();
	glm::vec3 currentPos = character->GetPosition();


	if (World::GetInstance().CheckForTreasure(actor, { cellSize.x - 1.f,cellSize.y - 1.f }))
	{
		if (character->GetDirection() == 0)
		{
			actor->SetPosition(currentPos.x, currentPos.y - 0.5f);
		}
		else
		{
			actor->SetPosition(currentPos.x, currentPos.y + 0.5f);
		}
		return true;
	}

	return false;
}

void dae::VerticalWalkState::CalculateDirection(GameObject* actor, CharacterComponent* character)
{

	glm::vec3 currentPos = actor->GetPosition().GetPosition();

	auto cell = character->GetCurrentCell();


	if (character->GetDirection() == 0)
	{
		if (currentPos.y >= cell->position.y)
		{

			character->CalculateCell();
		}
	}
	else
	{
		if (currentPos.y + 50.f <= cell->position.y)
		{
			character->CalculateCell();
		}
	}


}

//----------- IDLE --------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

void dae::IdleState::UpdateSprite(GameObject* , int )
{
	//actor->GetComponent<SimpleRenderComponent>().SetFlip(SDL_FLIP_NONE);
	//actor->GetComponent<SimpleRenderComponent>().SetAngle(0.f);
}


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


//----------- DEAD --------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

void dae::DeadState::UpdateSprite(GameObject* actor, int )
{
	SimpleRenderComponent& simpleRender = actor->GetComponent<SimpleRenderComponent>();
	simpleRender.SetAngleAndFlip(0.f, SDL_FLIP_NONE);
	simpleRender.SetTexture("../Data/Sprites/Death.png");

	actor->GetComponent<CharacterComponent>().GetDamaged();
}

dae::PlayerState* dae::DeadState::HandleInput(Command::InputType , PlayerStateType )
{
	return nullptr;
}

void dae::DeadState::Update(GameObject* , CharacterComponent* character)
{
	character->UpdateDeath();
}
