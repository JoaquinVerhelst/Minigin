#include "Command.h"
#include "GameObject.h"

#include "DiggerComponent.h"


namespace dae
{





	dae::WalkCommand::WalkCommand(int direction)
		:m_Direction{ direction }
	{
	}

	void WalkCommand::Execute(GameObject* actor)
	{
		actor->GetComponent<DiggerComponent>().Walk(m_Direction);
	}




}


