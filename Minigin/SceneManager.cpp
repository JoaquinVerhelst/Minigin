#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "GameObject.h"
#include "World.h"
#include <iostream>
#include <SDL.h>


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
	//std::cout << "NEXT LEVEL" << '\n';


	//for (size_t i = 0; i < m_scenes.size(); i++)
	//{
	//	m_scenes[i]->GetLevelInfo().w
	//}


	if (m_CurrentId == static_cast<int>(m_scenes.size() - 1))
	{
		m_CurrentId = 1;
	}
	else if (m_CurrentId >= static_cast<int>(m_scenes.size() - 1))
	{
		m_CurrentId = 0;
	}
	else
	{
		++m_CurrentId;
	}

	World::GetInstance().ResetAndLoadWorld(m_CurrentId);

}

void dae::SceneManager::NextSceneByIndex(int index)
{
	if (index >= static_cast<int>(m_scenes.size()))
	{
		m_CurrentId = 0;
		return;
	}

	m_CurrentId = index;


	World::GetInstance().ResetAndLoadWorld(m_CurrentId);
}

dae::LevelInfo* dae::SceneManager::GetSceneLevelInfo(int sceneIndex)
{
	return m_scenes[sceneIndex]->GetLevelInfo();
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetScene(int sceneIndex)
{
	return m_scenes[sceneIndex];
}

dae::LevelInfo* dae::SceneManager::GetCurrentSceneLevelInfo()
{
	return m_scenes[m_CurrentId]->GetLevelInfo();
}

std::shared_ptr <dae::Scene > dae::SceneManager::GetCurrentScene()
{
	return m_scenes[m_CurrentId];
}

std::vector<std::shared_ptr<dae::Scene>> dae::SceneManager::GetScenes()
{
	return m_scenes;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
