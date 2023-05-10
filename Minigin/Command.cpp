#include "Command.h"
#include "GameObject.h"

#include "CharacterComponent.h"
#include "State.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"


namespace dae
{
	dae::HorizontalWalkCommand::HorizontalWalkCommand(int direction)
		:m_Direction{ direction }
	{
	}

	void HorizontalWalkCommand::Execute(GameObject* actor, InputType inputType)
	{
		actor->GetComponent<CharacterComponent>().HorizontalWalk(m_Direction, inputType, StateType::HorizontalWalk);
	}



	VerticalWalkCommand::VerticalWalkCommand(int direction)
		:m_Direction{ direction }
	{
	}

	void VerticalWalkCommand::Execute(GameObject* actor, InputType inputType)
	{
		actor->GetComponent<CharacterComponent>().VerticalWalk(m_Direction, inputType, StateType::VerticalWalk);
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


