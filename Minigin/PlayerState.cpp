#include "PlayerState.h"
#include <iostream>
#include "SimpleRenderComponent.h"
#include "GameObject.h"
#include "World.h"
#include "CharacterComponent.h"
#include "DiggerComponent.h"

//-----------HORIZINTAL WALK-----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

void dae::HorizontalWalkState::UpdateSprite(GameObject* actor, Direction direction)
{

	SimpleRenderComponent& simpleRender = actor->GetComponent<SimpleRenderComponent>();

	if (direction == Direction::Right)
	{
		simpleRender.SetAngleAndFlip(0.f, SDL_FLIP_NONE);
	}
	else
	{
		simpleRender.SetAngleAndFlip(180.f, SDL_FLIP_VERTICAL);
	}
}

dae::CharacterState* dae::HorizontalWalkState::HandleInput(Command::InputType inputType, PlayerStateType newStateType)
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
	auto direction = character->GetDirection();

	glm::vec2 vec;

	if (direction == Direction::Right)
	{
		vec = character->CalculateWalk(0, character->GetCellSize().y, currentPos.y, currentPos.x);
	}
	else
	{
		vec = character->CalculateWalk(1, character->GetCellSize().y, currentPos.y, currentPos.x);
	}




	actor->SetPosition(vec.y, vec.x);

}

bool dae::HorizontalWalkState::CheckCollision(GameObject* actor, CharacterComponent* character)
{
	auto cellSize = character->GetCellSize();
	glm::vec3 currentPos = character->GetPosition();
	float offset = 0.5f;
	
	if (World::GetInstance().CheckForTreasure(actor, { cellSize.x - 1.f ,cellSize.y - 1.f }))
	{

		if (character->GetDirection() == Direction::Right)
		{
			actor->SetPosition(currentPos.x - offset, currentPos.y);
		}
		else
		{
			actor->SetPosition(currentPos.x + offset, currentPos.y);
		}
		return true;
	}

	return false;
}

void dae::HorizontalWalkState::CalculateDirection(GameObject* actor, CharacterComponent* character)
{
	glm::vec3 currentPos = actor->GetPosition().GetPosition();

	auto cell = character->GetCurrentCell();

	if (character->GetDirection() == Direction::Right)
	{
		if (currentPos.x >= cell->position.x)
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

void dae::VerticalWalkState::UpdateSprite(GameObject* actor, Direction direction)
{

	SimpleRenderComponent& simpleRender = actor->GetComponent<SimpleRenderComponent>();

	if (direction == Direction::Down)
	{
		simpleRender.SetAngleAndFlip(90.f, SDL_FLIP_NONE);
	}
	else
	{
		simpleRender.SetAngleAndFlip(270.f, SDL_FLIP_VERTICAL);
	}
}

dae::CharacterState* dae::VerticalWalkState::HandleInput(Command::InputType inputType, PlayerStateType newStateType)
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

	auto direction = character->GetDirection();

	glm::vec2 vec;

	if (direction == Direction::Down)
	{
		vec = character->CalculateWalk(0, character->GetCellSize().x, currentPos.x, currentPos.y);
	}
	else
	{
		vec = character->CalculateWalk(1, character->GetCellSize().x, currentPos.x, currentPos.y);
	}



	actor->SetPosition(vec.x, vec.y);
}

bool dae::VerticalWalkState::CheckCollision(GameObject* actor, CharacterComponent* character)
{
	auto cellSize = character->GetCellSize();
	glm::vec3 currentPos = character->GetPosition();
	float offset = 0.5f;

	if (World::GetInstance().CheckForTreasure(actor, { cellSize.x,cellSize.y }))
	{
		if (character->GetDirection() == Direction::Down)
		{
			actor->SetPosition(currentPos.x, currentPos.y - offset);
		}
		else
		{
			actor->SetPosition(currentPos.x, currentPos.y + offset);
		}
		return true;
	}

	return false;
}

void dae::VerticalWalkState::CalculateDirection(GameObject* actor, CharacterComponent* character)
{

	glm::vec3 currentPos = actor->GetPosition().GetPosition();

	auto cell = character->GetCurrentCell();


	if (character->GetDirection() == Direction::Down)
	{
		if (currentPos.y >= cell->position.y)
		{
			character->CalculateCell();
		}
	}
	else
	{
		if (currentPos.y <= cell->position.y - character->GetCellSize().x + 10.f)
		{
			character->CalculateCell();
		}
	}


}

//----------- IDLE --------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

void dae::IdleState::UpdateSprite(GameObject* , Direction)
{
	//actor->GetComponent<SimpleRenderComponent>().SetFlip(SDL_FLIP_NONE);
	//actor->GetComponent<SimpleRenderComponent>().SetAngle(0.f);
}


dae::CharacterState* dae::IdleState::HandleInput(Command::InputType , PlayerStateType newStateType)
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

bool dae::IdleState::CheckCollision(GameObject* actor, CharacterComponent* character)
{
	auto cellSize = character->GetCellSize();


	if (World::GetInstance().CheckForTreasure(actor, { cellSize.x ,cellSize.y  }))
	{
		return true;
	}

	return false;
}


//----------- DEAD --------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

void dae::DeadState::UpdateSprite(GameObject*, Direction )
{
}

dae::CharacterState* dae::DeadState::HandleInput(Command::InputType , PlayerStateType )
{
	return nullptr;
}

void dae::DeadState::Update(GameObject* , CharacterComponent* )
{
}


//----------- DAMAGED --------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////


void dae::DamagedState::UpdateSprite(GameObject* actor, Direction)
{
	SimpleRenderComponent& simpleRender = actor->GetComponent<SimpleRenderComponent>();
	simpleRender.SetAngleAndFlip(0.f, SDL_FLIP_NONE);
	simpleRender.SetTexture(actor->GetComponent<DiggerComponent>().GetSprites()->deathSprite);
}

dae::CharacterState* dae::DamagedState::HandleInput(Command::InputType, PlayerStateType)
{
	return nullptr;
}

void dae::DamagedState::Update(GameObject*, CharacterComponent* character)
{
	character->UpdateDeath();
}
