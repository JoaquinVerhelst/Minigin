#pragma once

#include <SDL.h>


#include "Singleton.h"
#include <map>
#include <memory>
#include "Command.h"



namespace dae
{
	enum class InputType {
		Pressed,
		Up,
		Down
	};

	class InputManager final : public Singleton<InputManager>
	{
		class InputManagerImpl;
		InputManagerImpl* pImpl;

	public:
		enum class ControllerButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			ButtonA = 0x1000,
			ButtonB = 0x2000,
			ButtonX = 0x4000,
			ButtonY = 0x8000
		};




		bool ProcessInput();

		bool IsPressed(ControllerButton button, int ID) const;
		bool IsDown(ControllerButton button, int ID) const;
		bool IsUp(ControllerButton button, int ID) const;


		int AssignController(GameObject* actor);

		explicit InputManager();
		~InputManager();


		void AddControllerBinding(ControllerButton button, Command* command, InputType inputType);
		void AddKeyBinding(SDL_Scancode button, Command* command, int inputID);

	};



}
