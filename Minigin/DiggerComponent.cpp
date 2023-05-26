#include "DiggerComponent.h"
#include "Time.h"
#include "InputManager.h"
#include "Command.h"
#include "GameObject.h"

namespace dae
{

	DiggerComponent::DiggerComponent(GameObject* owner, float walkSpeed)
		: dae::CharacterComponent(owner, walkSpeed, true)
	{
		SetState(new IdleState);
	}


	void DiggerComponent::Update()
	{
		CharacterComponent::Update();
	}


	void DiggerComponent::Init()
	{
		CharacterComponent::Init();
	}

	void DiggerComponent::Render() const
	{
		CharacterComponent::Render();
	}



}