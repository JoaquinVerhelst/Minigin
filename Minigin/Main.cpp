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

#include "World.h"
#include "EmeraldComponent.h"
#include "GoldComponent.h"

#include "Command.h"



using namespace dae;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//TODO Render Component and add it
	auto go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::SimpleRenderComponent>("../Data/background.tga", true);
	//go->GetComponent<dae::SimpleRenderComponent>().SetTextureBackgound();
	//scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::SimpleRenderComponent>("../Data/logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	//TODO text Component
	auto font1 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::SimpleRenderComponent>("Programming 4 Assignment", font1, false);
	go->SetPosition(80, 20);
	scene.Add(go);

	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::SimpleRenderComponent>( "fps", font2, true);
	go->AddComponent<dae::FPSComponent>();
	go->SetPosition(10, 10);
	scene.Add(go);



	//Player 1
	auto player1 = std::make_shared<dae::GameObject>();
	player1->AddComponent<DiggerComponent>(100.f);
	player1->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Player1.png");
	player1->GetComponent<SimpleRenderComponent>().CalculateScaleByGrid();
	player1->SetPosition(100, 100);

	player1->AddComponent<HealthComponent>(3);
	player1->AddComponent<HealthBarComponent>("../Data/Sprites/Player.png");

	player1->AddComponent<ScoreComponent>();
	player1->AddComponent<ScoreDisplayComponent>(font1);
	player1->GetComponent<ScoreDisplayComponent>().SetStartPosition(0, 60);
	scene.Add(player1);



	//Player 2
	auto player2 = std::make_shared<dae::GameObject>();
	player2->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Player1.png");
	player2->SetPosition(300, 100);
	player2->AddComponent<DiggerComponent>(100.f);
	player2->AddComponent<HealthComponent>(3);
	player2->AddComponent<HealthBarComponent>("../Data/Sprites/Player.png", true);
	player2->GetComponent<HealthBarComponent>().SetStartPosition(590, 0);
	player2->AddComponent<ScoreComponent>();
	player2->AddComponent<ScoreDisplayComponent>(font1);
	player2->GetComponent<ScoreDisplayComponent>().SetStartPosition(490, 60);
	scene.Add(player2);




	for (int i = 0; i < 20; i++)
	{
		go = std::make_shared<dae::GameObject>();
		go->AddComponent<SimpleRenderComponent>("../Data/Sprites/Emerald.png");
		go->AddComponent<EmeraldComponent>();

		scene.Add(go);
		World::GetInstance().PlaceTreasure(go, 10 + i * 10);

	}

	int index = 0;

	for (int i = 0; i < 5; i++)
	{
		index = 9 + i * 25;

		go = std::make_shared<dae::GameObject>();
		go->AddComponent<SimpleRenderComponent>("../Data/Sprites/Gold.png");
		go->AddComponent<GoldComponent>(index);

		scene.Add(go);
		World::GetInstance().PlaceTreasure(go, index );
	}

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}