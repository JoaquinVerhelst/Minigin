#include "NobbinManager.h"
#include "Time.h"

dae::NobbinManager::NobbinManager(GameObject* owner, int amountOfNobbin, float timeTospawnNobbin)
	: Component(owner)
	, m_Counter{ 0 }
	, m_TimeToSpawn{ timeTospawnNobbin }
	, m_AmountofNobbinLeft{ amountOfNobbin }
{
}

dae::NobbinManager::~NobbinManager()
{
}


void dae::NobbinManager::Update()
{
	m_Counter += Time::GetInstance().GetDeltaTime();

	if (m_Counter >= m_TimeToSpawn)
	{
		m_Counter = 0;

		SpawnNobbin();
	}


	if (IsOverlappingWithPlayer())
	{

	}


}

void dae::NobbinManager::Init()
{

}

void dae::NobbinManager::Render() const
{

}


void dae::NobbinManager::SpawnNobbin()
{
	
}

bool dae::NobbinManager::IsOverlappingWithPlayer()
{
	return false;
}
