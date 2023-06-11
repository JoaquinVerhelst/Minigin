#include "Command.h"
#include "GameObject.h"

#include "CharacterComponent.h"
#include "PlayerState.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"



#include "SoundServiceLocator.h"
#include "SoundSystem.h"

namespace dae
{
	dae::HorizontalWalkCommand::HorizontalWalkCommand(Direction direction)
		:m_Direction{ direction }
	{
	}

	void HorizontalWalkCommand::Execute(GameObject* actor, InputType inputType)
	{
		CharacterComponent& character = actor->GetComponent<CharacterComponent>();
		character.HandleInput(inputType, PlayerStateType::HorizontalWalk);
		character.SetDirection(m_Direction);
	}



	VerticalWalkCommand::VerticalWalkCommand(Direction direction)
		:m_Direction{ direction }
	{
	}

	void VerticalWalkCommand::Execute(GameObject* actor, InputType inputType)
	{
		CharacterComponent& character = actor->GetComponent<CharacterComponent>();
		character.HandleInput(inputType, PlayerStateType::VerticalWalk);
		character.SetDirection(m_Direction);
	}


	void MuteCommand::Execute(GameObject* , InputType type)
	{
		if (type == InputType::Up)
		{
			SoundServiceLocator::GetSoundSystem().MuteSound();
		}

	}

	void UseSpecialtyCommand::Execute(GameObject* actor, InputType type)
	{
		if (type == InputType::Up)
		{
			actor->GetComponent<CharacterComponent>().UseSpecialty();
		}
	}


	//void SelfDamageCommand::Execute(GameObject* actor, InputType inputType)
	//{
	//	if (inputType == InputType::Up)
	//	{
	//		HealthComponent& healthComp = actor->GetComponent<HealthComponent>();

	//		if (healthComp.GetHealth() == 0)
	//		{
	//			healthComp.SetHealth(0);
	//		}
	//		else
	//		{
	//			healthComp.SetHealth(healthComp.GetHealth() - 1);
	//		}






	//		SoundServiceLocator::GetSoundSystem().Play(0, 1.0f);



	//	}
	//}


	//void AddScoreCommand::Execute(GameObject* actor, InputType inputType)
	//{
	//	if (inputType == InputType::Up)
	//	{
	//		ScoreComponent& scoreComp = actor->GetComponent<ScoreComponent>();

	//		scoreComp.AddScore(100);

	//	}
	//}










}


