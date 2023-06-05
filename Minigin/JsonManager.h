#pragma once

#include <string>
#include "Scene.h"
#include "World.h"
#include "Singleton.h"

namespace dae {


	struct HighscoreEntry
	{
		HighscoreEntry()
		{
			name = "";
			score = 0;
		}
		std::string name;
		int score;
	};


	class JsonManager final : public Singleton<JsonManager>
	{

	public:
		bool LoadLevelsFromJson(const std::string& filePath);
		std::vector<HighscoreEntry> LoadHighscoresFromJson(const std::string& filePath);
		void SaveHighscoresToJson(const std::string& filePath, const std::vector<HighscoreEntry>& highscores);

		std::shared_ptr<GameObject> LoadPlayer();

		std::shared_ptr<GameObject> LoadNobbinPlayer();
	private:
			
		std::string m_JsonFilePath;
		int m_AmountOfPlayers = 0;
	};

}



