#pragma once



namespace dae
{


	class GameObject;


	class Component
	{


	public:

		GameObject* GetOwner() const { return m_pOwner; }

		void SetOwner(GameObject* pOwner)
		{
			if (m_pOwner != nullptr)
				m_pOwner = nullptr;

			m_pOwner = pOwner;
		}

		Component()
			:m_pOwner{}
		{};
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update() = 0;
		virtual void Init() = 0;
		virtual void Render() const = 0;
		virtual bool IsRenderer() = 0;

	protected:

		//explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {}

	private:

		GameObject* m_pOwner;
	};
}