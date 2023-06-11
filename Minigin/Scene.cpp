#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name), m_LevelInfo{ nullptr }
{}

Scene::~Scene()
{
	delete m_LevelInfo;
};

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_Objects.emplace_back(object);
}

void dae::Scene::SetLevelInfo(LevelInfo* levelInfo)
{
	m_LevelInfo = levelInfo;
}

LevelInfo* dae::Scene::GetLevelInfo()
{
	return m_LevelInfo;
}

void dae::Scene::AddTreasure(std::shared_ptr<GameObject> treasure)
{
	m_Treasures.emplace_back(treasure);
}

void dae::Scene::RemoveTreasures()
{
	m_Treasures.clear();
}


void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
}

void Scene::RemoveAll()
{
	m_Objects.clear();
	m_Treasures.clear();
}

void Scene::Update()
{

	for (auto& object : m_Objects)
	{
		object->Update();
	}

	for (auto& treasure : m_Treasures)
	{
		treasure->Update();
	}


	for (size_t i = 0; i < m_Objects.size(); ++i)
	{
		if (m_Objects[i]->IsDestroyed())
		{
			m_Objects.erase(m_Objects.begin() + i);
		}
	}
	for (size_t i = 0; i < m_Treasures.size(); ++i)
	{
		if (m_Treasures[i]->IsDestroyed())
		{
			m_Treasures.erase(m_Treasures.begin() + i);
		}
	}
}

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		object->FixedUpdate();
	}

	//for (auto& treasure : m_Treasures)
	//{
	//	treasure->FixedUpdate();
	//}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}

	for (auto& treasure : m_Treasures)
	{
		treasure->Render();
	}
}

