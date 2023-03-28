#pragma once

#include "GameObject.h"
#include <map>


namespace dae
{
	class GameObject;
	//enum class Direction : int
	//{
	//	Up,
	//	Down,
	//	Right,
	//	Left
	//};


	class Command
	{
	public:

		Command() = default;
		virtual ~Command() = default;
		virtual void Execute(GameObject* actor) = 0;

	};




	class WalkCommand : public Command
	{
	public:
		//0 = up, 1 = down, 2 = right, 3 = left
		WalkCommand(int direction);

		void Execute(GameObject* actor) override;


	private:

		int m_Direction;
	};








}