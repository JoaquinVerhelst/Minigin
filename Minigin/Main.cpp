#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"


#include "World.h"


#include "JsonManager.h"


#include <iostream>

using namespace dae;

void load()
{

	//World::GetInstance().ResetAndLoadWorld(0);


	JsonManager::GetInstance().LoadLevelsFromJson("../Data/JSON/DiggerLevels.json");

	World::GetInstance().ResetAndLoadWorld(0);


	////GENERAL ---------------------------------------------------------------------------------


	//auto UIDisplay = std::make_shared<dae::GameObject>();
	//UIDisplay->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/UI.png");

	//level1.Add(UIDisplay);
	//level2.Add(UIDisplay);
	//level3.Add(UIDisplay);


	//auto font1 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	//auto fps = std::make_shared<dae::GameObject>();
	//fps->AddComponent<dae::SimpleRenderComponent>("fps", font1, true);
	//fps->AddComponent<dae::FPSComponent>();
	//fps->SetPosition(10, 10);

	//menu.Add(fps);
	//level1.Add(fps);
	//level2.Add(fps);
	//level3.Add(fps);


	//auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	////Player 1
	//auto player1 = std::make_shared<dae::GameObject>();
	//player1->AddComponent<DiggerComponent>(100.f);
	//player1->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Player.png");
	//player1->AddComponent<HealthComponent>(3);
	//player1->AddComponent<HealthBarComponent>("../Data/Sprites/Player1.png");
	//player1->GetComponent<HealthBarComponent>().SetStartPosition(20, 51);
	//player1->AddComponent<ScoreComponent>();
	//player1->AddComponent<ScoreDisplayComponent>(font2);
	//player1->GetComponent<ScoreDisplayComponent>().SetStartPosition(20, 20);

	//level1.Add(player1);
	//level2.Add(player1);
	//level3.Add(player1);


	////Player 2
	//auto player2 = std::make_shared<dae::GameObject>();

	//player2->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Player.png");

	//player2->AddComponent<DiggerComponent>(100.f);
	//player2->AddComponent<HealthComponent>(3);
	//player2->AddComponent<HealthBarComponent>("../Data/Sprites/Player.png", true);
	//player2->GetComponent<HealthBarComponent>().SetStartPosition(990, 51);
	//player2->AddComponent<ScoreComponent>();
	//player2->AddComponent<ScoreDisplayComponent>(font2);
	//player2->GetComponent<ScoreDisplayComponent>().SetStartPosition(900, 20);

	//level1.Add(player2);
	//level2.Add(player2);
	//level3.Add(player2);






	////MENU -------------------------------------------------------------------------------------

	////background

	//auto menubackground = std::make_shared<dae::GameObject>();
	//menubackground->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Menu.png", true);
	//menu.Add(menubackground);





	//LevelInfo* menuLevelInfo = new LevelInfo();

	//menuLevelInfo->worldType = World::WorldTypes::Menu;
	//menuLevelInfo->background = menubackground; // "../Data/Sprites/Menu.png";
	//menuLevelInfo->player1 = player1;
	//menuLevelInfo->player2 = player2;

	//menuLevelInfo->brokenWorldIndexs;
	//menuLevelInfo->emeraldIndexs;
	//menuLevelInfo->goldIndexs;
	//menuLevelInfo->player1PosIndex;
	//menuLevelInfo->player2PosIndex;

	//menu.SetLevelInfo(menuLevelInfo);



	//// Level 1 ---------------------------------------------------------------------------------

	////background

	//auto level1background = std::make_shared<dae::GameObject>();
	//level1background->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Background1.png", true);
	//level1.Add(level1background);


	//LevelInfo* firstLevelInfo = new LevelInfo();

	//firstLevelInfo->worldType = World::WorldTypes::Level;
	//firstLevelInfo->background = level1background; // "../Data/Sprites/Background1.png";
	//firstLevelInfo->player1 = player1;
	//firstLevelInfo->player2 = player2;

	//firstLevelInfo->brokenWorldIndexs = std::vector<int>{ 17,18,19,20,21,22,36,37,38,52,66,80,94,95,96,110,124,138,152,166,180,194,193,192,191,190,189,188,187,186,200,214,228,242,256,270 };
	//firstLevelInfo->emeraldIndexs = std::vector<int>{ 26,27,41,74,75,76,77,88,89,90,91,144,145,146,147,148,140,154,217,218,231,232,245,246,234,235,248,249,263,262,265};
	//firstLevelInfo->goldIndexs = std::vector<int>{32,87,134,163,157,229};
	//firstLevelInfo->player1PosIndex = 110;
	//firstLevelInfo->player2PosIndex = 180;

	//level1.SetLevelInfo(firstLevelInfo);


	//// Level 2 ---------------------------------------------------------------------------------

	////background

	//auto level2background = std::make_shared<dae::GameObject>();
	//level2background->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Background2.png", true);
	//level2.Add(level2background);

	//LevelInfo* SecondLevelInfo = new LevelInfo();

	//SecondLevelInfo->worldType = World::WorldTypes::Level;
	//SecondLevelInfo->background = level2background; // "../Data/Sprites/Background2.png";
	//SecondLevelInfo->player1 = player1;
	//SecondLevelInfo->player2 = player2;

	//SecondLevelInfo->brokenWorldIndexs = std::vector<int>{3,4,5,6,7,8,9,10,11,12,13,14,17,20,28,31,34,42,45,48,49,50,56,59,64,70,73,78,79,84,87,93,98,101,107,108,109,110,111,112,121,126,135,140,149,154,163,168,171,177,178,179,180,181, 115,129,143,157,182,185,191,196,199,204,205,210,213,218,224,227,230,231,232,238,241,244,252,255,258,266,269,270,271,272,273,274,275,276,277,278,279,280 };
	//SecondLevelInfo->emeraldIndexs = std::vector<int>{ 36,37,38,39,40,54,68,60,74,75,89,90,102,103,104,116,117,118,130,132,144,146,158,159,160,172,173,174,187,188,200,201,214,123,124,136,139,150,153,165,166,246,247,248,249,250,136,222 };
	//SecondLevelInfo->goldIndexs = std::vector<int>{ 35,88,131,145,186 ,245};
	//SecondLevelInfo->player1PosIndex = 107;
	//SecondLevelInfo->player2PosIndex = 177;

	//level2.SetLevelInfo(SecondLevelInfo);


	//// Level 3 ---------------------------------------------------------------------------------

	////background

	//auto level3background = std::make_shared<dae::GameObject>();
	//level3background->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Background3.png", true);
	//level3.Add(level3background);


	//LevelInfo* thirdLevelInfo = new LevelInfo();

	//thirdLevelInfo->worldType = World::WorldTypes::Level;
	//thirdLevelInfo->background = level3background; // "../Data/Sprites/Background2.png";
	//thirdLevelInfo->player1 = player1;
	//thirdLevelInfo->player2 = player2;

	//thirdLevelInfo->brokenWorldIndexs = std::vector<int>{ 31,32,33,34,35,36,45,59,73,87,101,115,129,143,157,171,185,199,213,214,215,216,217,218,219,220,221,222,223,224,238,252,266,270,271,272,273,274,275,276,277,278,279,280 };
	//thirdLevelInfo->emeraldIndexs = std::vector<int>{4,5,6,60,62,64,66,68,74,76,78,80,82,88,90,92,94,96,102,104,106,108,110,130,132,134,136,138,144,146,148,150,152,158,160,162,164,166,172,174,176,178,180 };
	//thirdLevelInfo->goldIndexs = std::vector<int>{ 46,50,116,120,186,190,241};
	//thirdLevelInfo->player1PosIndex = 213;
	//thirdLevelInfo->player2PosIndex = 224;

	//level3.SetLevelInfo(thirdLevelInfo);






	//auto enemy = std::make_shared<dae::GameObject>();
	//enemy->AddComponent<SimpleRenderComponent>("../Data/Sprites/Enemy.png");
	//enemy->AddComponent<NobbinComponent>(100.f);
	//level1.Add(enemy);

	//World::GetInstance().PlaceGameObject(enemy, 269);


}

int main(int, char*[]) {
	dae::Minigin engine("");
	engine.Run(load);
    return 0;
}