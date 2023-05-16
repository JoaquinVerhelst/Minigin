#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "Command.h"

namespace dae
{
	//class Component;
	//class vector;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:

		void Update();
		void FixedUpdate();
		void Render() const;
		void Init();

		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;



		void Destroy() { m_IsDestroyed = true; }
		bool IsDestroyed() { return m_IsDestroyed; }

		//void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);
		Transform GetPosition() { return m_Transform; }
		
		void SetShape(glm::ivec2 shape) { m_Shape = shape; }
		glm::ivec2 GetShape() { return m_Shape; }



		//Component System
		template <typename T, typename...TArgs>
		T& AddComponent(TArgs&&... mArgs);

		template <typename T>
		T& GetComponent() const;

		template <typename T>
		void RemoveComponent();

		template <typename T>
		bool HasComponent() const;



		//Scenegraph
		void SetParent(std::shared_ptr<GameObject> parent, bool keepWorldPosition);
		std::shared_ptr<GameObject> GetParent() const { return m_Parent; }

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		void SetLocalPosition(const glm::vec3& pos);
		void SetPositionDirty();
		void UpdateWorldPosition();


		//DEBUG

		void SetDebug(bool active) { m_Debug = active; }

	private:

		void RemoveChild(std::shared_ptr<GameObject> go);
		void AddChild(std::shared_ptr<GameObject> go, bool keepWorldPosition);

		void UpdateTransform();

		std::vector<std::shared_ptr<Component>> m_Components;
		Transform m_Transform;
		glm::ivec2 m_Shape;
		bool m_IsDestroyed;
		bool m_CanRender;


		//Scenegraph
		std::vector<std::shared_ptr<GameObject>> m_Children;
		std::shared_ptr<GameObject> m_Parent;


		glm::vec3 m_LocalPosition;
		glm::vec3 m_WorldPosition;
		bool m_PositionIsDirty;




		//DEBUGG

		bool m_Debug = false;
	};


	

	template<typename T, typename...TArgs>
	inline T& GameObject::AddComponent(TArgs&&... mArgs)
	{

		if (HasComponent<T>()) {
			throw std::runtime_error("Error: Component of this Type" + std::string(typeid(T).name()) + "is already attached to this GameObject!");
		}

		std::shared_ptr<T> newComponent = std::make_shared<T>(this, std::forward<TArgs>( mArgs)...);
		//newComponent->SetOwner(this);
		m_Components.emplace_back(newComponent);
		newComponent->Init();

		return *newComponent;
	}

	template<typename T>
	T& GameObject::GetComponent() const
	{

		// Find the component of type T in the list of components
		auto it = std::find_if(m_Components.begin(), m_Components.end(), [](const auto& component) {
			return dynamic_cast<T*>(component.get()) != nullptr;
			});

		// If the component is found, return a reference to it
		if (it != m_Components.end()) {
			return dynamic_cast<T&>(**it);
		}

		// If the component is not found, throw an exception
		throw std::runtime_error("No component of this type: " + std::string(typeid(T).name()) + "is in this gameobject");
	}


	template<typename T> void GameObject::RemoveComponent()
	{
		// Find the component of type T in the list of components
		auto it = std::find_if(m_Components.begin(), m_Components.end(), [](const auto& component) {
			return dynamic_cast<T*>(component.get()) != nullptr;
			});

		// If the component is found, remove it from the list
		if (it != m_Components.end()) {
			(*it)->SetGameObject(nullptr);
			m_Components.erase(it);
		}
	}


	template<typename T> bool GameObject::HasComponent() const
	{
		// Check if a component of type T is attached to this game object
		return std::any_of(m_Components.begin(), m_Components.end(), [](const auto& component) {
			return dynamic_cast<T*>(component.get()) != nullptr;
			});
	}

}
