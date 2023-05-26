#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"

#include "GameObject.h"
#include "RotatorComponent.h"
#include "SimpleRenderComponent.h"
#include "FPSComponent.h"
#include "DiggerComponent.h"
#include "HealthBarComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "ScoreDisplayComponent.h"

#include "InputManager.h"

#include "NobbinComponent.h"
#include "World.h"
#include "EmeraldComponent.h"
#include "GoldComponent.h"

#include "Command.h"

#include "MenuComponent.h"



using namespace dae;

void load()
{


	auto& menu = dae::SceneManager::GetInstance().CreateScene("Menu");
	auto& level1 = dae::SceneManager::GetInstance().CreateScene("Level1");
	auto& level2 = dae::SceneManager::GetInstance().CreateScene("Level2");
	auto& level3 = dae::SceneManager::GetInstance().CreateScene("Level3");



	//GENERAL ---------------------------------------------------------------------------------

	auto font1 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	auto fps = std::make_shared<dae::GameObject>();
	fps->AddComponent<dae::SimpleRenderComponent>("fps", font1, true);
	fps->AddComponent<dae::FPSComponent>();
	fps->SetPosition(10, 10);
	menu.Add(fps);
	level1.Add(fps);
	level2.Add(fps);
	level3.Add(fps);


	//Player 1
	auto player1 = std::make_shared<dae::GameObject>();
	player1->AddComponent<DiggerComponent>(100.f);
	player1->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Player.png");

	player1->AddComponent<HealthComponent>(3);
	//player1->AddComponent<HealthBarComponent>("../Data/Sprites/Player1.png");

	player1->AddComponent<ScoreComponent>();
	player1->AddComponent<ScoreDisplayComponent>(font1);
	player1->GetComponent<ScoreDisplayComponent>().SetStartPosition(0, 60);

	level1.Add(player1);
	level2.Add(player1);
	level3.Add(player1);

	World::GetInstance().PlaceGameObject(player1, 30);



	//MENU -------------------------------------------------------------------------------------

	//background

	auto menubackground = std::make_shared<dae::GameObject>();
	menubackground->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Menu.png", true);
	menu.Add(menubackground);


	auto menuButtons = std::make_shared<dae::GameObject>();
	menuButtons->AddComponent<dae::MenuComponent>();
	menu.Add(menuButtons);


	LevelInfo* menuLevelInfo = new LevelInfo();

	menuLevelInfo->levelIndex = 0;
	menuLevelInfo->background = menubackground; // "../Data/Sprites/Menu.png";

	World::GetInstance().AddLevelInfo(menuLevelInfo);


	// Level 1 ---------------------------------------------------------------------------------

	//background

	auto level1background = std::make_shared<dae::GameObject>();
	level1background->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Background1.png", true);
	level1.Add(level1background);

	LevelInfo* firstLevelInfo = new LevelInfo();
	firstLevelInfo->levelIndex = 1;
	firstLevelInfo->background = level1background;// "../Data/Sprites/Background1.png";
	firstLevelInfo->player1 = player1;
	World::GetInstance().AddLevelInfo(firstLevelInfo);

	// Level 2 ---------------------------------------------------------------------------------

	//background

	auto level2background = std::make_shared<dae::GameObject>();
	level2background->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Background2.png", true);
	level2.Add(level2background);


	LevelInfo* SecondLevelInfo = new LevelInfo();
	SecondLevelInfo->levelIndex = 2;
	SecondLevelInfo->background = level2background; // "../Data/Sprites/Background2.png";
	SecondLevelInfo->player1 = player1;
	World::GetInstance().AddLevelInfo(SecondLevelInfo);



	// Level 3 ---------------------------------------------------------------------------------

	//background

	auto level3background = std::make_shared<dae::GameObject>();
	level3background->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Background3.png", true);
	level3.Add(level3background);

	LevelInfo* thirdLevelInfo = new LevelInfo();
	thirdLevelInfo->levelIndex = 3;
	thirdLevelInfo->background = level3background; // "../Data/Sprites/Background3.png";
	thirdLevelInfo->player1 = player1;
	World::GetInstance().AddLevelInfo(thirdLevelInfo);



	//go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/SpritesSheetFull.png", true);
	////go->SetPosition(216, 180);
	//scene2.Add(go);





	//auto font1 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	////go = std::make_shared<dae::GameObject>();
	////go->AddComponent<dae::SimpleRenderComponent>("Programming 4 Assignment", font1, false);
	////go->SetPosition(80, 20);
	////scene.Add(go);


	//scene2.Add(go);


	////Player 1
	//auto player1 = std::make_shared<dae::GameObject>();
	//player1->AddComponent<DiggerComponent>(100.f);
	//player1->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Player1.png");
	////player1->GetComponent<SimpleRenderComponent>().CalculateScaleByGrid();
	////player1->SetPosition(100, 100);

	//player1->AddComponent<HealthComponent>(3);
	//player1->AddComponent<HealthBarComponent>("../Data/Sprites/Player.png");

	//player1->AddComponent<ScoreComponent>();
	//player1->AddComponent<ScoreDisplayComponent>(font1);
	//player1->GetComponent<ScoreDisplayComponent>().SetStartPosition(0, 60);
	//scene.Add(player1);
	//World::GetInstance().PlaceGameObject(player1, 30);




	////Player 2
	//auto player2 = std::make_shared<dae::GameObject>();

	////player2->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Player1.png");
	//player2->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Enemy.png");
	//// 
	//// 
	////player2->SetPosition(300, 100);
	//player2->AddComponent<DiggerComponent>(100.f);
	//player2->AddComponent<HealthComponent>(3);
	//player2->AddComponent<HealthBarComponent>("../Data/Sprites/Player.png", true);
	//player2->GetComponent<HealthBarComponent>().SetStartPosition(590, 0);
	//player2->AddComponent<ScoreComponent>();
	//player2->AddComponent<ScoreDisplayComponent>(font1);
	//player2->GetComponent<ScoreDisplayComponent>().SetStartPosition(490, 60);
	//scene2.Add(player2);
	//World::GetInstance().PlaceGameObject(player2, 100);





	////NOBBIN



	//auto nobbin = std::make_shared<dae::GameObject>();

	//nobbin->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Enemy.png");
	//nobbin->AddComponent<NobbinComponent>();
	////nobbin->GetComponent<SimpleRenderComponent>().CalculateScaleByGrid();

	//scene.Add(nobbin);
	//World::GetInstance().PlaceGameObject(nobbin, 59);





	////WORLD

	auto go = std::make_shared<dae::GameObject>();


	//for (int i = 0; i < 20; i++)
	//{
	//	 go = std::make_shared<dae::GameObject>();
	//	go->AddComponent<SimpleRenderComponent>("../Data/Sprites/Emerald.png");
	//	go->AddComponent<EmeraldComponent>();

	//	level1.Add(go);
	//	World::GetInstance().PlaceTreasure(go, 10 + i * 10);

	//}

	int index = 0;

	for (int i = 0; i < 5; i++)
	{
		index = 9 + i * 25;

		 go = std::make_shared<dae::GameObject>();
		go->AddComponent<SimpleRenderComponent>("../Data/Sprites/Gold.png");
		go->AddComponent<GoldComponent>(index);

		level1.Add(go);
		World::GetInstance().PlaceTreasure(go, index );
	}

	//std::vector<int> indexs = { 59,60,61,62,63,64,65,66,67,73,81,87,95,101,102,103,104,
	//	105,106,107,108,109,115,123,129,130,131,132,133,134,135,136,137,143,147,151,157,
	//	161,165,171,175,179,185,189,193,199,200,201,202,203,204,205,206,207,213,227,241,
	//	255,269 };

	//World::GetInstance().BreakGridIndexes(indexs);





	World::GetInstance().ResetAndLoadWorld(0);










	//make a function for each object.. like CreatePlayer(with all the params )

	//CreatePlayer() {

	// new go = ... 
	// go->AddComponent<....>()



	// so the json file, needs the type of gameobject, params, for each comp
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}