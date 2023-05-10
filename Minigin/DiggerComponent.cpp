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

	}


	void DiggerComponent::Update()
	{
		CharacterComponent::Update();
	}


	void DiggerComponent::Init()
	{
	}

	void DiggerComponent::Render() const
	{
		CharacterComponent::Render();
	}



}