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

		enum class InputType
		{
			Pressed,
			Down,
			Up
		};



		Command() = default;
		virtual ~Command() = default;
		virtual void Execute(GameObject* actor, InputType type) = 0;

	};




	class HorizontalWalkCommand : public Command
	{
	public:
		//0 = left, 1 = right
		HorizontalWalkCommand(int direction);

		void Execute(GameObject* actor, InputType type) override;


	private:

		int m_Direction;
	};



	class VerticalWalkCommand : public Command
	{
	public:
		//0 = up, 1 = down
		VerticalWalkCommand(int direction);

		void Execute(GameObject* actor, InputType type) override;


	private:

		int m_Direction;
	};


	//class SelfDamageCommand : public Command
	//{
	//public:

	//	SelfDamageCommand() = default;

	//	void Execute(GameObject* actor, InputType type) override;


	//private:
	//};



	//class AddScoreCommand : public Command
	//{
	//public:

	//	AddScoreCommand() = default;

	//	void Execute(GameObject* actor, InputType type) override;


	//private:
	//};


}