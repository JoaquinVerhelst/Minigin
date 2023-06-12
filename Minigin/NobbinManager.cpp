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
	, m_GridCell{ nullptr }
	, m_NobbinPlayer{nullptr}
	, m_Nobbins{nullptr}
	, m_Players{ nullptr }
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


	SpawnNobbin();


	if (m_NobbinAmountLeft == 0 && m_Nobbins.empty() && World::GetInstance().GetGameMode() != World::GameModeTypes::Versus)
	{
		SceneManager::GetInstance().NextScene();
	}



	CalculateNobbins();


	
	if (m_NobbinPlayer != nullptr)
	{
		CalculateNobbinPlayer();
	}



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
	m_NobbinPlayer = nullptr;
}

void dae::NobbinManager::Disable()
{
	m_Nobbins.clear();
	m_NobbinAmountLeft = 0;
	m_NobbinPlayer = nullptr;
	m_Players = World::GetInstance().GetPlayers();
}

std::vector<std::shared_ptr<dae::GameObject>> dae::NobbinManager::GetNobbins()
{
	return m_Nobbins;
}

void dae::NobbinManager::DestroyNobbin(std::shared_ptr<dae::GameObject> nobbin)
{
	SoundServiceLocator::GetSoundSystem().Play(0, 128.f);

	for (size_t i = 0; i < m_Nobbins.size(); i++)
	{
		if (m_Nobbins[i] == nobbin)
		{
			m_Nobbins.erase(m_Nobbins.begin() + i);
		}
	}


}

void dae::NobbinManager::SetNobbinPlayer(std::shared_ptr<GameObject> nobbinPlayer)
{
	m_NobbinPlayer = nobbinPlayer;
}

void dae::NobbinManager::CalculateNobbins()
{
	for (size_t n = 0; n < m_Nobbins.size(); ++n)
	{
		for (size_t p = 0; p < m_Players.size(); ++p)
		{
			IsOverlappingWithPlayer(m_Nobbins[n], m_Players[p]);
		}

		if (IsOverlappingWithTreasure(m_Nobbins[n]))
		{
			for (size_t j = 0; j < m_Players.size(); ++j)
			{
				m_Players[j]->GetComponent<ScoreComponent>().AddEmeraldScore(250);
			}

			DestroyNobbin(m_Nobbins[n]);
		}
	}

}

void dae::NobbinManager::CalculateNobbinPlayer()
{
	IsOverlappingWithPlayer(m_NobbinPlayer, m_Players[0]);

	if (IsOverlappingWithTreasure(m_NobbinPlayer))
	{
		m_Players[0]->GetComponent<ScoreComponent>().AddEmeraldScore(250);
		
		World::GetInstance().PlaceGameObject(m_NobbinPlayer, static_cast<int>(World::GetInstance().GetWorldGrid().size() - 1));
	}
}

void dae::NobbinManager::SpawnNobbin()
{
	m_Counter += Time::GetInstance().GetDeltaTime();

	if (m_Counter >= m_TimeToSpawn)
	{
		m_Counter = 0;

		if (m_NobbinAmountLeft >= 1)
		{
			std::shared_ptr<GameObject> nobbin = JsonManager::GetInstance().LoadNobbin();

			nobbin->SetPosition(m_GridCell->position.x, m_GridCell->position.y);

			m_Nobbins.emplace_back(nobbin);
			--m_NobbinAmountLeft;
		}

	}
}

bool dae::NobbinManager::IsOverlappingWithPlayer(std::shared_ptr<GameObject> nobbin, std::shared_ptr<GameObject> player)
{

	auto cellSize = World::GetInstance().GetCellSize();


	
	const glm::vec2& playerpos = player->GetPosition().GetPosition();


	glm::vec2 nobbinPos = nobbin->GetPosition().GetPosition();

	nobbinPos = glm::vec2(nobbinPos.x + 5.f, nobbinPos.y + 5.f);

	glm::vec2 nobbinSize = glm::vec2(cellSize.x - 10.f, cellSize.y - 10.f);

	if (playerpos.x + cellSize.x > nobbinPos.x && playerpos.x < nobbinPos.x + nobbinSize.x &&
		playerpos.y + cellSize.y > nobbinPos.y && playerpos.y < nobbinPos.y + nobbinSize.y)
	{
		player->GetComponent<DiggerComponent>().GetDamaged();
		return true;
	}
		
	

	return false;
}

bool dae::NobbinManager::IsOverlappingWithTreasure(std::shared_ptr<GameObject> nobbin)
{

	auto cellSize = World::GetInstance().GetCellSize();


	auto treasure = World::GetInstance().GetIsOverlappingTreasure(nobbin->GetPosition().GetPosition(), { cellSize.x ,cellSize.y });

	if (treasure == nullptr)
		return false;

	if (treasure->HasComponent<GoldComponent>())
	{
		GoldComponent* gold = &treasure->GetComponent<GoldComponent>();

		if ( gold->GetStateType() == GoldStateType::Falling)
		{

			return true;

		}
	}

	
	

	return false;
}
