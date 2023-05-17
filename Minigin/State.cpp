#include "State.h"
#include <iostream>

bool dae::HorizontalWalkState::HandleInput(CharacterComponent* character, Command::InputType inputType, StateType newStateType)
{

	if (character->GetState()->GetType() == newStateType)
	{
		if (inputType == Command::InputType::Pressed)
		{
			return true;
		}
		else if (inputType == Command::InputType::Up)
		{
			character->SetState(new IdleState);
			//std::cout << "idle state" << '\n';
			return false;
		}
	}
	else
	{
		return false;
	}

	//else if ( newStateType == ( a State that can be transitioned to))
	//{
	//	character->SetState(new state);
	//	return false;
	//}

	return false;
}



bool dae::VerticalWalkState::HandleInput(CharacterComponent* character, Command::InputType inputType, StateType newStateType)
{

	if (character->GetState()->GetType() == newStateType)
	{
		if (inputType == Command::InputType::Pressed)
		{
			return true;
		}
		else if (inputType == Command::InputType::Up)
		{
			character->SetState(new IdleState);
			//std::cout << "idle state" << '\n';
			return false;
		}
	}
	else
	{
		return false;
	}


	return false;
}


bool dae::IdleState::HandleInput(CharacterComponent* character, Command::InputType , StateType newStateType)
{
	if (newStateType == StateType::HorizontalWalk)
	{
		character->SetState(new HorizontalWalkState);
		//std::cout << "horizontal walk state" << '\n';
		return true;
	}
	else if (newStateType == StateType::VerticalWalk)	
	{
		character->SetState(new VerticalWalkState);
		//std::cout << "vertical walk state" << '\n';
		return true;
	}
	else
	{
		return false;
	}
}


