#pragma once



namespace dae
{
	class GameObject;
	//class Event;
	
	enum class Event
	{
		PlayerDie,
		PlayerWin,
		PlayerHealthChanged,
		PlayerScoreChanged,
		NobbinDied
	};



	class Observer
	{
	public:
		virtual ~Observer() {}
		virtual void OnNotify(const GameObject& entity, Event event) = 0;
	};


}


