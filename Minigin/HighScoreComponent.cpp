#include "HighScoreComponent.h"

#include "Font.h"
#include "ResourceManager.h"
#include <SDL_ttf.h>
#include "Renderer.h"
#include "InputManager.h"
#include "ScoreComponent.h"
#include <algorithm>

dae::HighScoreComponent::HighScoreComponent(GameObject* owner)
	:Component(owner)
	, m_FilePath{"../Data/JSON/HighScore.json"}
	, m_DisplayText{ " " }
	, m_NameText{" "}
	, m_TextBox{nullptr}
	, m_PlayersSaved{}
	, m_DoOnce{ true }
{

}

dae::HighScoreComponent::~HighScoreComponent()
{
	//delete m_TextBox;
}

void dae::HighScoreComponent::Update()
{
	bool gameOver = World::GetInstance().IsGameLost();




	if (gameOver)
	{
		if (m_DoOnce)
		{

			auto players = World::GetInstance().GetPlayers();

			m_DisplayText = "Score: " + std::to_string(players[0]->GetComponent<ScoreComponent>().GetScore()) + "  Player1: ";
			m_DoOnce = false;
			m_TextBox->SetText(m_DisplayText + m_NameText);
		}


		CheckInput();
	}







}

void dae::HighScoreComponent::Init()
{
	UpdateHighScoreList();
	m_TextBox->SetText(m_DisplayText + m_NameText);
}

void dae::HighScoreComponent::Render() const
{
	for (size_t i = 0; i < m_TextRenders.size(); i++)
	{
		m_TextRenders[i]->Render();
	}
}



void dae::HighScoreComponent::CheckInput()
{


	auto& input = InputManager::GetInstance();

	std::string text = input.GetPressedKeyString();

	if (!text.empty())
	{

		if (m_NameText.length() <= 5)
		{
			m_NameText += text;
			m_TextBox->SetText(m_DisplayText + m_NameText);
		}


	}
	else if (input.IsKeyBoardKey(SDLK_RETURN, SDL_KEYUP))
	{
		SaveHighScore();

	}
	else if (input.IsKeyBoardKey(SDLK_BACKSPACE, SDL_KEYUP))
	{
		size_t length =  m_NameText.length() - 1;
		m_NameText.erase(length);
		m_TextBox->SetText(m_DisplayText + m_NameText);
	}

}

void dae::HighScoreComponent::CheckPlayers()
{


}

void dae::HighScoreComponent::UpdateHighScoreList()
{
	m_TextRenders.clear();

	auto highScoreCoop = JsonManager::GetInstance().LoadHighscoresForCoop(m_FilePath);
	auto highScoreSinglePlayer = JsonManager::GetInstance().LoadHighscoresForSinglePlayer(m_FilePath);

	auto font = ResourceManager::GetInstance().LoadFont("../Data/Lingua.otf", 30);

	//Sort
	std::sort(highScoreSinglePlayer.begin(), highScoreSinglePlayer.end(), [](const HighscoreEntry& a, const HighscoreEntry& b)
		{
			return a.score > b.score;
		});

	std::sort(highScoreCoop.begin(), highScoreCoop.end(), [](const HighscoreEntryCoop& a, const HighscoreEntryCoop& b)
		{
			int totalScore1 = a.score1 + a.score2;
			int totalScore2 = b.score1 + b.score2;
			return totalScore1 > totalScore2;
		});

	GetOwner()->AddComponent<SimpleRenderComponent>(m_NameText, font, false).SetPosition({ 400.f, 600.f });
	m_TextBox = &GetOwner()->GetComponent<SimpleRenderComponent>();


	float offset = 50.f;

	for (size_t i = 0; i < highScoreSinglePlayer.size(); ++i)
	{

		auto go = std::make_shared<GameObject>();

		std::string scoreText = std::to_string(highScoreSinglePlayer[i].score) + "             " + highScoreSinglePlayer[i].name;

		go->AddComponent<SimpleRenderComponent>(scoreText, font, false).SetPosition({ 150.f, 200.f + offset * i });


		m_TextRenders.emplace_back(go);

	}


	for (size_t i = 0; i < highScoreCoop.size(); ++i)
	{
		auto go = std::make_shared<GameObject>();

		std::string scoreText = std::to_string(highScoreCoop[i].score1) + ": " + highScoreCoop[i].name1 + "        "
			+ std::to_string(highScoreCoop[i].score2) + ": " + highScoreCoop[i].name2;

		go->AddComponent<SimpleRenderComponent>(scoreText, font, false).SetPosition({ 600.f, 200.f + offset * i });

		m_TextRenders.emplace_back(go);
	}

}


void dae::HighScoreComponent::AddHighScore(std::string name, int score)
{
	HighscoreEntry entry;
	entry.name = name;
	entry.score = score;
	m_CurrentHighScore = entry;
}

void dae::HighScoreComponent::AddHighScoreCoop(std::string name, int score, int index)
{
	if (index == 0)
	{
		m_CurrentHighScoreCoop.name1 = name;
		m_CurrentHighScoreCoop.score1 = score;

	}
	else
	{
		m_CurrentHighScoreCoop.name2 = name;
		m_CurrentHighScoreCoop.score2 = score;
	}
}


void dae::HighScoreComponent::SaveHighScore()
{
	auto& world = World::GetInstance();

	auto players = world.GetPlayers();
	++m_PlayersSaved;

	if (m_PlayersSaved == static_cast<int>(players.size()))
	{


		if (m_PlayersSaved == 2)
		{
			AddHighScoreCoop(m_NameText, players[1]->GetComponent<ScoreComponent>().GetScore(), 1);
			JsonManager::GetInstance().SaveHighscoresToCoop(m_FilePath, m_CurrentHighScoreCoop);
		}
		else
		{
			AddHighScore(m_NameText, players[0]->GetComponent<ScoreComponent>().GetScore());
			JsonManager::GetInstance().SaveHighscoresToSinglePlayer(m_FilePath, m_CurrentHighScore);
		}

		m_PlayersSaved = 0;
		world.SetGameLost(false);
		m_DisplayText = " ";
		m_NameText = " ";
		m_DoOnce = true;
		m_TextBox->SetText(m_DisplayText + m_NameText);

		UpdateHighScoreList();
	}
	else
	{
		AddHighScoreCoop(m_NameText, players[0]->GetComponent<ScoreComponent>().GetScore(), 0);

		m_NameText = " ";
		m_DisplayText = "Score: " + std::to_string(players[1]->GetComponent<ScoreComponent>().GetScore()) + "  Player2: ";
		m_TextBox->SetText(m_DisplayText + m_NameText);
	}
}
