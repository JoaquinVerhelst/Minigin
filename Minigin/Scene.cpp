#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	//TODO Add lateUpdate or Double buffer pattern

	for(size_t i = 0; i < m_objects.size(); ++i)
	{
		m_objects[i]->Update();

		if (m_objects[i]->IsDestroyed())
		{
			m_Indexs.emplace_back(i);
		}
	}

	if (m_Indexs.size() > 0)
	{
		for (size_t i = 0; i < m_Indexs.size(); i++)
		{
			m_objects[m_Indexs[i]]->~GameObject();
		}
	}

}

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

