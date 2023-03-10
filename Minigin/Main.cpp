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

using namespace dae;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//TODO Render Component and add it
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::SimpleRenderComponent>("../Data/background.tga");
	scene.Add(go);

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


	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Player.png");
	go->SetPosition(100, 100);
	go->AddComponent<RotatorComponent>(glm::vec2{100, 100}, 20.f, 5.f);
	scene.Add(go);


	auto child = std::make_shared<dae::GameObject>();
	child->AddComponent<dae::SimpleRenderComponent>("../Data/Sprites/Enemy.png");
	//child->AddComponent<RotatorComponent>(glm::vec2{ 0, 0 }, 20.f, 5.f);
	go->SetPosition(10, 10);

	scene.Add(child);

	child->SetParent(go, true);
	child->SetDebug(true);




}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}