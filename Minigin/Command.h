#pragma once

#include "GameObject.h"
#include <map>
//#include "InputManager.h"

namespace dae
{
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
		virtual void Execute(GameObject* actor, int type) = 0;

	};




	class WalkCommand : public Command
	{
	public:
		//0 = up, 1 = down, 2 = right, 3 = left
		WalkCommand(int direction);

		void Execute(GameObject* actor, int type) override;


	private:

		int m_Direction;
	};


	class SelfDamageCommand : public Command
	{
	public:

		SelfDamageCommand() = default;

		void Execute(GameObject* actor, int type) override;


	private:
	};



	class AddScoreCommand : public Command
	{
	public:

		AddScoreCommand() = default;

		void Execute(GameObject* actor, int type) override;


	private:
	};


}