#pragma once
//#include "GameObject.h"


namespace dae
{


	class GameObject;


	class Component
	{

		GameObject* m_pOwner;
	public:
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update() = 0;
		virtual void Init() {};
		virtual void Render() const {};
		virtual bool IsRenderer() = 0;

	protected:
		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {}
		GameObject* GetOwner() const { return m_pOwner; }


	};
}