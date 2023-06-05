#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"


namespace dae
{
	class Scene;
	class GameObject;
	struct LevelInfo;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void FixedUpdate();
		void Render();

		void NextScene();
		void NextSceneByIndex(int index);


		LevelInfo* GetSceneLevelInfo(int sceneIndex);
		std::shared_ptr<Scene> GetScene(int sceneIndex);

		LevelInfo* GetCurrentSceneLevelInfo();
		std::shared_ptr<Scene> GetCurrentScene();

		std::vector<std::shared_ptr<Scene>> GetScenes();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		int m_CurrentId = 0;
	};
}
