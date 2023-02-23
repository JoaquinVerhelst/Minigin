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

#include "RenderTextComponent.h"
#include "SimpleRenderComponent.h"
#include "FPSComponent.h"

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
	go->AddComponent<dae::RenderTextComponent>("Programming 4 Assignment", font1, false);
	go->SetPosition(80, 20);
	scene.Add(go);

	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderTextComponent>( "fps", font2, true);
	go->AddComponent<dae::FPSComponent>();
	go->SetPosition(10, 10);
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}