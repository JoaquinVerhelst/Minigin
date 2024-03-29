#pragma once
#include "SceneManager.h"
#include <vector>


namespace dae
{
	class GameObject;

	enum class WorldTypes
	{
		Default,
		Menu,
		Level,
		HighScore
	};



	struct LevelInfo
	{
		LevelInfo()
		{
			name = "";
			background = nullptr;
			worldType = WorldTypes::Default;
			player1 = nullptr;
			player1PosIndex = 0;
			player2 = nullptr;
			player2PosIndex = 0;
			brokenWorldIndexs = {};
			emeraldIndexs = {};
			goldIndexs = {};
			nobbinManagerPos = 0;
			nobbinAmount = 0;
			nobbinManager = nullptr;
		}


		std::string name;
		std::string backgroundFilepath;

		WorldTypes worldType;

		int player1PosIndex;
		int player2PosIndex;

		std::vector<int> brokenWorldIndexs;
		std::vector<int> emeraldIndexs;
		std::vector<int> goldIndexs;


		std::shared_ptr<GameObject> background;
		std::shared_ptr<GameObject> player1;
		std::shared_ptr<GameObject> player2;
		std::shared_ptr<GameObject> nobbinManager;

		int nobbinManagerPos;
		int nobbinAmount;
		
	};

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);



		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		
		void Update();
		void FixedUpdate();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void SetLevelInfo(LevelInfo* levelInfo);
		LevelInfo* GetLevelInfo();
		void AddTreasure(std::shared_ptr<GameObject> treasure);
		void RemoveTreasures();


	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector < std::shared_ptr<GameObject>> m_Objects{};
		std::vector < std::shared_ptr<GameObject>> m_Treasures{};

		LevelInfo* m_LevelInfo;
		static unsigned int m_idCounter; 
	};

}
