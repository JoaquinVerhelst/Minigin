#pragma once

#include "Observer.h"
#include "Component.h"
#include "Singleton.h"
#include <vector>

namespace dae
{
	class BaseComponent;
	//class Event;



	class Subject
	{
	public:

		void addObserver(Observer* observer)
		{
			m_Observers.emplace_back(observer);
		}

		void removeObserver(Observer* observer)
		{
			auto it = std::find(m_Observers.begin(), m_Observers.end(), observer);
			if (it != m_Observers.end())
				m_Observers.erase(it);
		}

		void removeAllObservers()
		{
			m_Observers.clear();
		}

		void notify(GameObject& entity, Event event)
		{
			for (size_t i = 0; i < m_Observers.size(); i++)
			{
				m_Observers[i]->OnNotify(entity, event);
			}
		}

	private:
		std::vector<Observer*> m_Observers;


	};


}