#include "State.h"
#include <iostream>

bool dae::WalkState::HandleInput(CharacterComponent* character, StateType newStateType, int inputType)
{

	if (character->GetState()->GetType() == newStateType)
	{
		if (inputType == 0)
		{
			return true;
		}
		else if (inputType == 1)
		{
			character->SetState(new IdleState);
			std::cout << "idle state" << '\n';
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

bool dae::IdleState::HandleInput(CharacterComponent* character, StateType newStateType, int )
{
	if (newStateType == StateType::Walk)
	{
		character->SetState(new WalkState);
		std::cout << "walk state" << '\n';
		return true;
	}
	//else if ( newStateType == ( a State that can be transitioned to))
	//{
	//	character->SetState(new state);
	//	return false;
	//}
	else
	{
		return false;
	}
}
