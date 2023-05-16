#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_Objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::Update()
{
	//TODO Add lateUpdate or Double buffer pattern

	for(size_t i = 0; i < m_Objects.size(); ++i)
	{
		m_Objects[i]->Update();

	}



	for (size_t i = 0; i < m_Objects.size(); ++i)
	{
		if (m_Objects[i]->IsDestroyed())
		{
			m_Objects.erase(m_Objects.begin() + i);
		}
	}

}

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

