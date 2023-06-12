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
	enum class Direction;

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




	class HorizontalWalkCommand final : public Command
	{
	public:
		//0 = left, 1 = right
		HorizontalWalkCommand(Direction direction);

		void Execute(GameObject* actor, InputType type) override;


	private:

		Direction m_Direction;
	};



	class VerticalWalkCommand final : public Command
	{
	public:
		//0 = up, 1 = down
		VerticalWalkCommand(Direction direction);

		void Execute(GameObject* actor, InputType type) override;


	private:

		Direction m_Direction;
	};

	class MuteCommand final : public Command
	{
	public:
		//0 = up, 1 = down

		void Execute(GameObject* actor, InputType type) override;
	};

	class UseSpecialtyCommand final : public Command
	{
	public:
		//0 = up, 1 = down

		void Execute(GameObject* actor, InputType type) override;
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