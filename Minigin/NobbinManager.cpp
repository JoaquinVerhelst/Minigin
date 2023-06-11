#include "NobbinManager.h"
#include "Time.h"
#include "SimpleRenderComponent.h"
#include "NobbinComponent.h"
#include "JsonManager.h"
#include "DiggerComponent.h"
#include "GoldComponent.h"
#include "ScoreComponent.h"
#include "SoundServiceLocator.h"

dae::NobbinManager::NobbinManager(GameObject* owner, int amountOfNobbin, int gridIndex, float timeTospawnNobbin)
	: Component(owner)
	, m_Counter{ 0 }
	, m_TimeToSpawn{ timeTospawnNobbin }
	, m_NobbinAmountLeft{ amountOfNobbin }
	, m_NobbinAmount{ amountOfNobbin }
	, m_GridIndex{ gridIndex }
{
}

dae::NobbinManager::~NobbinManager()
{
}


void dae::NobbinManager::Update()
{

	for (size_t i = 0; i < m_Nobbins.size(); i++)
	{
		m_Nobbins[i]->Update();
	}

	m_Counter += Time::GetInstance().GetDeltaTime();

	if (m_Counter >= m_TimeToSpawn)
	{
		m_Counter = 0;

		if (m_NobbinAmountLeft >= 1)
		{
			SpawnNobbin();
			--m_NobbinAmountLeft;
		}

	}

	if (m_NobbinAmountLeft == 0 && m_Nobbins.empty())
	{
		SceneManager::GetInstance().NextScene();
	}

	IsOverlappingWithPlayer();

	IsOverlappingWithTreasure();

}

void dae::NobbinManager::Init()
{
	auto grid = World::GetInstance().GetWorldGrid();
	m_GridCell = grid[m_GridIndex];
}

void dae::NobbinManager::Render() const
{
	for (size_t i = 0; i < m_Nobbins.size(); i++)
	{
		m_Nobbins[i]->Render();
	}
}


void dae::NobbinManager::Reset()
{
	m_Nobbins.clear();
	m_NobbinAmountLeft = m_NobbinAmount;
	m_Players = World::GetInstance().GetPlayers();
}

void dae::NobbinManager::Disable()
{
	m_Nobbins.clear();
	m_NobbinAmountLeft = 0;
}

void dae::NobbinManager::SpawnNobbin()
{
	
	std::shared_ptr<GameObject> nobbin = JsonManager::GetInstance().LoadNobbin();

	nobbin->SetPosition(m_GridCell->position.x, m_GridCell->position.y);

	m_Nobbins.emplace_back(nobbin);
}

bool dae::NobbinManager::IsOverlappingWithPlayer()
{

	auto cellSize = World::GetInstance().GetCellSize();

	for (size_t p = 0; p < m_Players.size(); ++p)
	{
		const glm::vec2& playerpos = m_Players[p]->GetPosition().GetPosition();

		for (size_t n = 0; n < m_Nobbins.size(); ++n)
		{
			const glm::vec2& nobbin = m_Nobbins[n]->GetPosition().GetPosition();


			if (playerpos.x + cellSize.x > nobbin.x && playerpos.x < nobbin.x + cellSize.x &&
				playerpos.y + cellSize.y > nobbin.y && playerpos.y < nobbin.y + cellSize.y)
			{
				m_Players[p]->GetComponent<DiggerComponent>().GetDamaged();
				return true;
			}
		}
	}

	return false;
}

bool dae::NobbinManager::IsOverlappingWithTreasure()
{

	auto cellSize = World::GetInstance().GetCellSize();

	for (size_t i = 0; i < m_Nobbins.size(); ++i)
	{
		auto treasure = World::GetInstance().GetIsOverlappingTreasure(m_Nobbins[i]->GetPosition().GetPosition(), { cellSize.x ,cellSize.y });

		if (treasure && treasure->GetComponent<GoldComponent>().GetStateType() == GoldStateType::Falling)
		{

			for (size_t j = 0; j < m_Players.size(); ++j)
			{
				m_Players[j]->GetComponent<ScoreComponent>().AddEmeraldScore(250);
			}

			SoundServiceLocator::GetSoundSystem().Play(0, 128.f);
			m_Nobbins.erase(m_Nobbins.begin() + i);

			return true;

		}
	}

	return false;
}
