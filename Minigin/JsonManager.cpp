#include "JsonManager.h"



#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "ResourceManager.h"
#include "SimpleRenderComponent.h" 
#include "FPSComponent.h"
#include "GameObject.h"
#include "MenuComponent.h"

#include "DiggerComponent.h"
#include "HealthBarComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "ScoreDisplayComponent.h"
#include "NobbinComponent.h"
#include "HighScoreComponent.h"
using json = nlohmann::json;


namespace dae
{


	bool dae::JsonManager::LoadLevelsFromJson(const std::string& filePath)
	{

        m_JsonFilePath = filePath;


        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::cout << "Failed to open JSON file: " << filePath << std::endl;
            return false;
        }

        try
        {
            json jsonData;
            file >> jsonData;


            const auto& generalInfo = jsonData["general"];

            ////GENERAL ---------------------------------------------------------------------------------

            auto UIDisplay = std::make_shared<dae::GameObject>();
            UIDisplay->AddComponent<SimpleRenderComponent>(generalInfo["UIDisplaySpritepath"]);


            auto menuButtons = std::make_shared<dae::GameObject>();
            menuButtons->AddComponent<MenuComponent>();



            auto highScore = std::make_shared<dae::GameObject>();
            highScore->AddComponent<HighScoreComponent>();



            const auto& fpsInfo = generalInfo["fps"];

            auto font1 = ResourceManager::GetInstance().LoadFont(fpsInfo["font"]["fontpath"], fpsInfo["font"]["size"]);
            auto fps = std::make_shared<GameObject>();
            fps->AddComponent<SimpleRenderComponent>("fps", font1, true);
            fps->AddComponent<FPSComponent>();
            fps->SetPosition(fpsInfo["position"][0], fpsInfo["position"][1]);

            //// ---------------------------------------------------------------------------------

            const auto& levels = jsonData["levels"];

            for (size_t i = 0; i < levels.size(); i++)
            {
                const json& levelInfoJson = levels[i]["levelInfo"];


                auto& level = dae::SceneManager::GetInstance().CreateScene(levelInfoJson["name"]);


                //add general stuff based on type
                // 



                LevelInfo* currentLevel = new LevelInfo();

                currentLevel->name = levelInfoJson["name"];
                currentLevel->backgroundFilepath = levelInfoJson["backgroundFilepath"];

                auto levelbackground = std::make_shared<dae::GameObject>();
                levelbackground->AddComponent<dae::SimpleRenderComponent>(currentLevel->backgroundFilepath, true);
                level.Add(levelbackground);

                currentLevel->background = levelbackground;

                const json& brokenWorldIndexsJson = levelInfoJson["brokenWorldIndexs"];
                for (const auto& index : brokenWorldIndexsJson) {
                    currentLevel->brokenWorldIndexs.emplace_back(index);
                }

                const json& emeraldIndexsJson = levelInfoJson["emeraldIndexs"];
                for (const auto& index : emeraldIndexsJson) {
                    currentLevel->emeraldIndexs.emplace_back(index);
                }


                const json& goldIndexsJson = levelInfoJson["goldIndexs"];
                for (const auto& index : goldIndexsJson) {
                    currentLevel->goldIndexs.emplace_back(index);
                }


                currentLevel->player1PosIndex = levelInfoJson["player1PosIndex"];
                currentLevel->player2PosIndex = levelInfoJson["player2PosIndex"];



                std::string worldType = levelInfoJson["worldType"];


                if (worldType == "Menu")
                {
                    level.Add(menuButtons);


                    currentLevel->worldType = World::WorldTypes::Menu;

                }
                else if (worldType == "Level")
                {
                    level.Add(UIDisplay);


                    currentLevel->worldType = World::WorldTypes::Level;
                }
                else if (worldType == "HighScore")
                {
                    level.Add(highScore);

                    currentLevel->worldType = World::WorldTypes::HighScore;
                }






                level.Add(fps);

                level.SetLevelInfo(currentLevel);

            }



            return true;
        }
        catch (const json::exception& ex)
        {
            std::cout << "Failed to parse JSON file: " << filePath << std::endl;
            std::cout << "Error: " << ex.what() << std::endl;
            return false;
        }
	}

    std::vector<HighscoreEntry> JsonManager::LoadHighscoresForSinglePlayer(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::cout << "Failed to open JSON file: " << filePath << std::endl;
            return std::vector<HighscoreEntry>();
        }

        try
        {
            json jsonData;
            file >> jsonData;

            std::vector<HighscoreEntry> highscores;

            // Extract highscores from JSON
            const json& entries = jsonData["singleplayer"];
            for (const auto& entry : entries)
            {
                HighscoreEntry highscore;
                highscore.name = entry["name"];
                highscore.score = entry["score"];
                highscores.push_back(highscore);
            }

            return highscores;
        }
        catch (const json::exception& ex)
        {
            std::cout << "Failed to parse JSON file: " << filePath << std::endl;
            std::cout << "Error: " << ex.what() << std::endl;
            return std::vector<HighscoreEntry>();
        }
    }

    std::vector<HighscoreEntryCoop> JsonManager::LoadHighscoresForCoop(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::cout << "Failed to open JSON file: " << filePath << std::endl;
            return std::vector<HighscoreEntryCoop>();
        }

        try
        {
            json jsonData;
            file >> jsonData;

            std::vector<HighscoreEntryCoop> highscores;

            // Extract highscores from JSON
            const json& entries = jsonData["coop"];
            for (const auto& entry : entries)
            {
                HighscoreEntryCoop highscore;

                highscore.name1 = entry["name1"];
                highscore.score1 = entry["score1"];

                highscore.name2 = entry["name2"];
                highscore.score2 = entry["score2"];

                highscores.push_back(highscore);
            }

            return highscores;
        }
        catch (const json::exception& ex)
        {
            std::cout << "Failed to parse JSON file: " << filePath << std::endl;
            std::cout << "Error: " << ex.what() << std::endl;
            return std::vector<HighscoreEntryCoop>();
        }
    }

    void JsonManager::SaveHighscoresToSinglePlayer(const std::string& filePath, const HighscoreEntry& highscores)
    {
        json jsonData;

        // Load existing JSON data from file
        std::ifstream inputFile(filePath);
        if (inputFile.is_open())
        {
            inputFile >> jsonData;
            inputFile.close();
        }
        else
        {
            std::cout << "Failed to open JSON file for loading: " << filePath << std::endl;
            return;
        }

        // Convert highscores to JSON
        json entryJson;
        entryJson["name"] = highscores.name;
        entryJson["score"] = highscores.score;
        jsonData["singleplayer"].push_back(entryJson);

        // Write updated JSON data to file
        std::ofstream outputFile(filePath);
        if (outputFile.is_open())
        {
            outputFile << jsonData.dump(4); // Write JSON data to file with indentation
            outputFile.close();
            std::cout << "Highscores saved to JSON file: " << filePath << std::endl;
        }
        else
        {
            std::cout << "Failed to open JSON file for saving: " << filePath << std::endl;
        }
    }

    void JsonManager::SaveHighscoresToCoop(const std::string& filePath, const HighscoreEntryCoop& highscores)
    {
        
        json jsonData;

        // Load existing JSON data from file
        std::ifstream inputFile(filePath);
        if (inputFile.is_open())
        {
            inputFile >> jsonData;
            inputFile.close();
        }
        else
        {
            std::cout << "Failed to open JSON file for loading: " << filePath << std::endl;
            return;
        }

        // Convert highscores to JSON
        json entryJson;

        entryJson["name1"] = highscores.name1;
        entryJson["score1"] = highscores.score1;
        entryJson["name2"] = highscores.name2;
        entryJson["score2"] = highscores.score2;

        jsonData["coop"].push_back(entryJson);

        // Write updated JSON data to file
        std::ofstream outputFile(filePath);
        if (outputFile.is_open())
        {
            outputFile << jsonData.dump(8); // Write JSON data to file with indentation
            outputFile.close();
            std::cout << "Highscores saved to JSON file: " << filePath << std::endl;
        }
        else
        {
            std::cout << "Failed to open JSON file for saving: " << filePath << std::endl;
        }
    }


    std::shared_ptr<GameObject> JsonManager::LoadPlayer(int index)
    {


        std::ifstream file(m_JsonFilePath);
        if (!file.is_open())
        {
            std::cout << "Failed to open JSON file: " << m_JsonFilePath << std::endl;
        }

        try
        {
            json jsonData;
            file >> jsonData;


            const auto& player = jsonData["general"]["player"];

            auto player1 = std::make_shared<dae::GameObject>();
            player1->AddComponent<DiggerComponent>(player["speed"]);
            player1->AddComponent<dae::SimpleRenderComponent>(player["spritepath"]);

            const json health = player["health"];

            player1->AddComponent<HealthComponent>(health["amount"]);
            player1->AddComponent<HealthBarComponent>(health["healthBarSpritepath"]);

            json pos;

            if (index == 0)
                pos = health["healthBarPosition1"];
            else
                pos = health["healthBarPosition2"];
            
            player1->GetComponent<HealthBarComponent>().SetStartPosition(pos[0], pos[1]);


            const json& score = player["score"];
            auto font2 = dae::ResourceManager::GetInstance().LoadFont(score["fontpath"], score["size"]);

            player1->AddComponent<ScoreComponent>();
            player1->AddComponent<ScoreDisplayComponent>(font2);


            if (index == 0)
                pos = score["position1"];
            else
                pos = score["position2"];


            player1->GetComponent<ScoreDisplayComponent>().SetStartPosition(pos[0], pos[1]);

            return player1;

        }
        catch (const json::exception& ex)
        {
            std::cout << "Failed to parse JSON file: " << m_JsonFilePath << std::endl;
            std::cout << "Error: " << ex.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<GameObject> JsonManager::LoadNobbinPlayer()
    {

        std::ifstream file(m_JsonFilePath);
        if (!file.is_open())
        {
            std::cout << "Failed to open JSON file: " << m_JsonFilePath << std::endl;
        }

        try
        {
            json jsonData;
            file >> jsonData;


            const auto& playerEnemyJson = jsonData["general"]["playerEnemy"];

            auto enemyPlayer = std::make_shared<dae::GameObject>();

            enemyPlayer->AddComponent<dae::SimpleRenderComponent>(playerEnemyJson["spritepath"]);
            enemyPlayer->AddComponent<NobbinComponent>(playerEnemyJson["speed"], true);
            const json health = playerEnemyJson["health"];

            enemyPlayer->AddComponent<HealthComponent>(health["amount"]);
            enemyPlayer->AddComponent<HealthBarComponent>(health["healthBarSpritepath"]);



            json pos = health["healthBarPosition"];

            enemyPlayer->GetComponent<HealthBarComponent>().SetStartPosition(pos[0], pos[1]);


            const json score = playerEnemyJson["score"];
            auto font2 = dae::ResourceManager::GetInstance().LoadFont(score["fontpath"], score["size"]);

            enemyPlayer->AddComponent<ScoreComponent>();
            enemyPlayer->AddComponent<ScoreDisplayComponent>(font2);



            pos = score["position"];



            enemyPlayer->GetComponent<ScoreDisplayComponent>().SetStartPosition(pos[0], pos[1]);

            return enemyPlayer;

        }
        catch (const json::exception& ex)
        {
            std::cout << "Failed to parse JSON file: " << m_JsonFilePath << std::endl;
            std::cout << "Error: " << ex.what() << std::endl;
            return nullptr;
        }
    }





}