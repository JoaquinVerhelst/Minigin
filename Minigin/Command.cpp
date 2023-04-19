#include "Command.h"
#include "GameObject.h"

#include "CharacterComponent.h"
#include "State.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"


namespace dae
{





	dae::WalkCommand::WalkCommand(int direction)
		:m_Direction{ direction }
	{
	}

	void WalkCommand::Execute(GameObject* actor, int type)
	{
		actor->GetComponent<CharacterComponent>().Walk(m_Direction, type, StateType::Walk);
	}




	void SelfDamageCommand::Execute(GameObject* actor, int inputType)
	{
		if (inputType == 1)
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


	void AddScoreCommand::Execute(GameObject* actor, int inputType)
	{
		if (inputType == 1)
		{
			ScoreComponent& scoreComp = actor->GetComponent<ScoreComponent>();

			scoreComp.AddScore(100);
		}

		
	}

}


