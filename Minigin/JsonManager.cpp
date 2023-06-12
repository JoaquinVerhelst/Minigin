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
#include "NobbinManager.h"

#include "GoldComponent.h"
#include "EmeraldComponent.h"
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

                currentLevel->nobbinManagerPos = levelInfoJson["nobbinManagerPos"];
                currentLevel->nobbinAmount = levelInfoJson["nobbinAmount"];


                std::string worldType = levelInfoJson["worldType"];


                if (worldType == "Menu")
                {
                    level.Add(menuButtons);


                    currentLevel->worldType = WorldTypes::Menu;

                }
                else if (worldType == "Level")
                {
                    level.Add(UIDisplay);
                    currentLevel->worldType = WorldTypes::Level;



                    auto nobbinManager = std::make_shared<dae::GameObject>();
                    nobbinManager->AddComponent<NobbinManager>(levelInfoJson["nobbinAmount"], levelInfoJson["nobbinManagerPos"]);

                    currentLevel->nobbinManager = nobbinManager;

                    level.Add(nobbinManager);
                }
                else if (worldType == "HighScore")
                {
                    level.Add(highScore);

                    currentLevel->worldType = WorldTypes::HighScore;
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


    std::shared_ptr<GameObject> JsonManager::LoadGold(int worldIndex)
    {
        std::ifstream file(m_JsonFilePath);
        if (!file.is_open())
        {
            std::cout << "Failed to open JSON file: " << m_JsonFilePath << std::endl;
            return std::shared_ptr<GameObject>{};
        }

        try
        {
            json jsonData;
            file >> jsonData;


            const auto& goldInfo = jsonData["general"]["gold"];

            auto gold = std::make_shared<dae::GameObject>();
            gold->AddComponent<SimpleRenderComponent>(goldInfo["sprites"]["goldBagSprite"]);
            gold->AddComponent<GoldComponent>(worldIndex, goldInfo["pushSpeed"], goldInfo["fallSpeed"]);

            gold->GetComponent<GoldComponent>().SetSprites(goldInfo["sprites"]["goldBrokenSprite"]);


            return gold;
        }
        catch (const json::exception& ex)
        {
            std::cout << "Failed to parse JSON file: " << m_JsonFilePath << std::endl;
            std::cout << "Error: " << ex.what() << std::endl;
            return std::shared_ptr<GameObject>{};
        }
    }


    std::shared_ptr<GameObject> JsonManager::LoadEmerald()
    {
        std::ifstream file(m_JsonFilePath);
        if (!file.is_open())
        {
            std::cout << "Failed to open JSON file: " << m_JsonFilePath << std::endl;
            return std::shared_ptr<GameObject>{};
        }

        try
        {
            json jsonData;
            file >> jsonData;


            const auto& emeraldInfo = jsonData["general"]["emerald"];

            auto emerald = std::make_shared<dae::GameObject>();
            emerald->AddComponent<SimpleRenderComponent>(emeraldInfo["sprite"]);
            emerald->AddComponent<EmeraldComponent>();


            return emerald;
        }
        catch (const json::exception& ex)
        {
            std::cout << "Failed to parse JSON file: " << m_JsonFilePath << std::endl;
            std::cout << "Error: " << ex.what() << std::endl;
            return std::shared_ptr<GameObject>{};
        }
    }
    std::shared_ptr<GameObject> JsonManager::LoadNobbin()
    {

        std::ifstream file(m_JsonFilePath);
        if (!file.is_open())
        {
            std::cout << "Failed to open JSON file: " << m_JsonFilePath << std::endl;
            return std::shared_ptr<GameObject>{};
        }

        try
        {
            json jsonData;
            file >> jsonData;


            const auto& nobbinInfo = jsonData["general"]["nobbin"];

            auto nobbin = std::make_shared<dae::GameObject>();
            nobbin->AddComponent<dae::SimpleRenderComponent>(nobbinInfo["nobbinSprite"]);
            nobbin->AddComponent<NobbinComponent>(nobbinInfo["speed"]);

;
            NobbinComponent::NobbinSprites* sprites = new NobbinComponent::NobbinSprites();
            sprites->enemySprite = nobbinInfo["nobbinSprite"];
            sprites->rageSprite = nobbinInfo["hobbinSprite"];

            nobbin->GetComponent<NobbinComponent>().SetSprites(sprites);


            return nobbin;
        }
        catch (const json::exception& ex)
        {
            std::cout << "Failed to parse JSON file: " << m_JsonFilePath << std::endl;
            std::cout << "Error: " << ex.what() << std::endl;
            return std::shared_ptr<GameObject>{};
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
            player1->AddComponent<dae::SimpleRenderComponent>(player["sprites"]["playerSprite"]);

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


            DiggerComponent::PlayerSprites* sprites = new DiggerComponent::PlayerSprites();
            sprites->playerSprite = player["sprites"]["playerSprite"];
            sprites->deathSprite = player["sprites"]["deathSprite"];
            sprites->reloadingSprite = player["sprites"]["reloadingSprite"];
            sprites->fireBallSprite = player["sprites"]["fireBallSprite"];


            player1->GetComponent<DiggerComponent>().SetSprites(sprites);


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



            enemyPlayer->AddComponent<dae::SimpleRenderComponent>(playerEnemyJson["sprites"]["enemySprite"]);
            enemyPlayer->AddComponent<NobbinComponent>(playerEnemyJson["speed"], true);


            NobbinComponent::NobbinSprites* sprites = new NobbinComponent::NobbinSprites();
            sprites->enemySprite = playerEnemyJson["sprites"]["enemySprite"];
            sprites->rageSprite = playerEnemyJson["sprites"]["rageSprite"];

            enemyPlayer->GetComponent<NobbinComponent>().SetSprites(sprites);


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