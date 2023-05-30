#include "EmeraldComponent.h"

#include "SoundServiceLocator.h"
#include "ScoreComponent.h"


dae::EmeraldComponent::EmeraldComponent(GameObject* owner)
	: TreasureComponent(owner)
{
}

void dae::EmeraldComponent::Update()
{
}

void dae::EmeraldComponent::FixedUpdate()
{
}

void dae::EmeraldComponent::Init()
{
}

bool dae::EmeraldComponent::PickUpTreasure(GameObject* actor)
{
	SoundServiceLocator::GetSoundSystem().Play(1, 1.0f);

	actor->GetComponent<ScoreComponent>().AddEmeraldScore(25);

	return true;
}

bool dae::EmeraldComponent::HandleCollision(GameObject* actor)
{
	return PickUpTreasure(actor);
}
