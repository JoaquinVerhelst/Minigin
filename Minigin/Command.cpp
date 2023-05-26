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
	dae::HorizontalWalkCommand::HorizontalWalkCommand(int direction)
		:m_Direction{ direction }
	{
	}

	void HorizontalWalkCommand::Execute(GameObject* actor, InputType inputType)
	{
		actor->GetComponent<CharacterComponent>().HandleInput(inputType, PlayerStateType::HorizontalWalk);
		actor->GetComponent<CharacterComponent>().SetDirection(m_Direction);
	}



	VerticalWalkCommand::VerticalWalkCommand(int direction)
		:m_Direction{ direction }
	{
	}

	void VerticalWalkCommand::Execute(GameObject* actor, InputType inputType)
	{
		actor->GetComponent<CharacterComponent>().HandleInput(inputType, PlayerStateType::VerticalWalk);
		actor->GetComponent<CharacterComponent>().SetDirection(m_Direction);
	}




	void SelfDamageCommand::Execute(GameObject* actor, InputType inputType)
	{
		if (inputType == InputType::Up)
		{
			HealthComponent& healthComp = actor->GetComponent<HealthComponent>();

			if (healthComp.GetHealth() == 0)
			{
				healthComp.SetHealth(0);
			}
			else
			{
				healthComp.SetHealth(healthComp.GetHealth() - 1);
			}






			SoundServiceLocator::GetSoundSysyem().Play(0, 1.0f);



		}
	}


	void AddScoreCommand::Execute(GameObject* actor, InputType inputType)
	{
		if (inputType == InputType::Up)
		{
			ScoreComponent& scoreComp = actor->GetComponent<ScoreComponent>();

			scoreComp.AddScore(100);

		}
	}






}


