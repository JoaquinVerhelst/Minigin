#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"
#include <iostream>
#include <SDL.h>
#include "World.h"

void dae::SceneManager::Update()
{
	//for(auto& scene : m_scenes)
	//{
	//	scene->Update();
	//}

	if (m_CurrentId != 0 && InputManager::GetInstance().IsKeyBoardKey(SDL_SCANCODE_F1, SDL_KEYUP) )
	{
		NextScene();
	}


	m_scenes[m_CurrentId]->Update();


}

void dae::SceneManager::FixedUpdate()
{
	//for (auto& scene : m_scenes)
	//{
	//	scene->FixedUpdate();
	//}

	m_scenes[m_CurrentId]->FixedUpdate();
}

void dae::SceneManager::Render()
{
	//for (const auto& scene : m_scenes)
	//{
	//	scene->Render();
	//}

	m_scenes[m_CurrentId]->Render();
}

void dae::SceneManager::NextScene()
{
	std::cout << "NEXT LEVEL" << '\n';


	++m_CurrentId;

	if (m_CurrentId >= m_scenes.size())
	{
		m_CurrentId = 0;

	}

	World::GetInstance().ResetAndLoadWorld(m_CurrentId);

}

void dae::SceneManager::NextSceneByIndex(int index)
{
	if (index >= m_scenes.size())
	{
		m_CurrentId = 0;
		return;
	}

	m_CurrentId = index;

}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
