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

	struct HighscoreEntryCoop
	{
		HighscoreEntryCoop()
		{
			name1 = "";
			score1 = 0;
			name2 = "";
			score2 = 0;
		}
		std::string name1;
		int score1;
		std::string name2;
		int score2;
	};


	class JsonManager final : public Singleton<JsonManager>
	{

	public:
		bool LoadLevelsFromJson(const std::string& filePath);

		std::vector<HighscoreEntry> LoadHighscoresForSinglePlayer(const std::string& filePath);
		std::vector<HighscoreEntryCoop> LoadHighscoresForCoop(const std::string& filePath);

		void SaveHighscoresToSinglePlayer(const std::string& filePath, const HighscoreEntry& highscores);
		void SaveHighscoresToCoop(const std::string& filePath, const HighscoreEntryCoop& highscore);




		std::shared_ptr<GameObject> LoadNobbin();

		std::shared_ptr<GameObject> LoadPlayer(int index);

		std::shared_ptr<GameObject> LoadNobbinPlayer();
	private:
			
		std::string m_JsonFilePath;
	};

}



