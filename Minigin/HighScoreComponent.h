#pragma once

#include "Texture2D.h"
#include "Component.h"
#include "SDL.h"
#include "GameObject.h"
#include "JsonManager.h"
#include "SimpleRenderComponent.h"

namespace dae
{

	class HighScoreComponent final : public Component
	{
	public:
		HighScoreComponent(GameObject* owner);
		~HighScoreComponent();

		HighScoreComponent(const HighScoreComponent& other) = delete;
		HighScoreComponent& operator=(const HighScoreComponent& other) = delete;
		HighScoreComponent(HighScoreComponent&& other) = delete;
		HighScoreComponent& operator=(HighScoreComponent&& other) = delete;


		void Update() override;
		void Init() override;
		void Render() const override;
		bool IsRenderer() override { return true; }


		//void HitButtons();


		//void ProcessMouseEvents();


		//bool IsPointInRect(const glm::ivec2& p, const SDL_Rect& r);



	private:


		void AddHighScore(std::string name, int score);
		void AddHighScoreCoop(std::string name, int score, int index);


		void SaveHighScore();

		void CheckInput();
		void CheckPlayers();
		void UpdateHighScoreList();

		std::string m_DisplayText;
		std::string m_NameText;
		SimpleRenderComponent* m_TextBox;

		HighscoreEntry m_CurrentHighScore;
		HighscoreEntryCoop m_CurrentHighScoreCoop;

		std::vector<HighscoreEntry> m_HighScoreSinglePlayer;
		std::vector<HighscoreEntryCoop> m_HighScoreCoop;

		std::string m_FilePath;
		std::vector<std::shared_ptr<GameObject>> m_TextRenders;

		//Font m_Font;

		int m_PlayersSaved;
		bool m_DoOnce;


	};


}

