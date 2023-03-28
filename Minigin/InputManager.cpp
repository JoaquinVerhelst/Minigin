#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>


#include "InputManager.h"
#include "ImGui/imgui_impl_sdl2.h"

#include <vector>

#include <iostream>
namespace dae
{



	class InputManager::InputManagerImpl
	{

	private:

		struct KeyBind
		{
			Command* command;
			SDL_Scancode button;
			int inputID;
		};


		XINPUT_STATE* m_PreviousStates;
		XINPUT_STATE* m_CurrentStates;

		WORD* m_ButtonsPressedThisFrame;
		WORD* m_ButtonsReleasedThisFrame;

		std::vector<GameObject*> m_Players{};

		std::vector<std::pair<ControllerButton, Command*>> m_ControllerBinds;

		std::vector<KeyBind> m_KeyBinds;

	public:
		InputManagerImpl()
			:m_CurrentStates(new XINPUT_STATE[XUSER_MAX_COUNT]), m_PreviousStates(new XINPUT_STATE[XUSER_MAX_COUNT])
			, m_ButtonsPressedThisFrame(new WORD[XUSER_MAX_COUNT]), m_ButtonsReleasedThisFrame(new WORD[XUSER_MAX_COUNT])
		{
		}

		bool ProcessInput()
		{
			std::swap(m_PreviousStates, m_CurrentStates);


			DWORD result;

			ZeroMemory(m_CurrentStates, sizeof(XINPUT_STATE) * XUSER_MAX_COUNT);

			for (DWORD i = 0; i < m_Players.size(); i++)
			{
				result = XInputGetState(i, &m_CurrentStates[i]);

				auto buttonChanges = m_CurrentStates[i].Gamepad.wButtons ^ m_PreviousStates[i].Gamepad.wButtons;
				m_ButtonsPressedThisFrame[i] = buttonChanges & m_CurrentStates[i].Gamepad.wButtons;
				m_ButtonsReleasedThisFrame[i] = buttonChanges & (~m_CurrentStates[i].Gamepad.wButtons);
			}



			ControllersHandler();


			KeyboardHandler();



			return KeyBoardEventhandler();
		}

		bool IsDownThisFrame(unsigned int button, int ID) const
		{
			return m_ButtonsPressedThisFrame[ID] & button;
		}
		bool IsUpThisFrame(unsigned int button, int ID) const
		{
			return m_ButtonsReleasedThisFrame[ID] & button;
		}
		bool IsPressed(unsigned int button, int ID) const
		{
			return m_CurrentStates[ID].Gamepad.wButtons & button;
		}


		//float GetThumbStickX(int ID) const
		//{
		//	//magic number = analog thumb stick max
		//	return fmaxf(-1, static_cast<float>(m_CurrentStates[ID].Gamepad.sThumbRX / 32767.00000f));
		//}
		//float GetThumbStickY(int ID) const
		//{
		//	//magic number = analog thumb stick max
		//	return fmaxf(-1, static_cast<float>(m_CurrentStates[ID].Gamepad.sThumbRY / 32767.00000f));
		//}


		~InputManagerImpl()
		{
			delete m_PreviousStates;
			delete m_CurrentStates;

			delete m_ButtonsPressedThisFrame;
			delete m_ButtonsReleasedThisFrame;
		}



		int AssignController(GameObject* actor)
		{
			if (m_Players.size() < XUSER_MAX_COUNT - 1)
			{
				m_Players.emplace_back(actor);

				return static_cast<int>(m_Players.size());
			}

			return -1;
		}

		

		// TODO: Think about how to seperate these down, up and pressed functions, since now its all at the same time
		// (Maybe in the command itself or the controllerbinds, like a extra variable??? same for keybaord)



		void ControllersHandler()
		{

			for (size_t i = 0; i < m_Players.size(); ++i)
			{
				for (size_t y = 0; y < m_ControllerBinds.size(); ++y)
				{
					const auto& key = m_ControllerBinds[y].first;


					if (IsDownThisFrame(static_cast<unsigned int>(key), static_cast<int>(i)))
					{
						auto& command = m_ControllerBinds[y].second;
						command->Execute(m_Players[i]);
					}
					else if (IsUpThisFrame(static_cast<unsigned int>(key), static_cast<int>(i)))
					{
						auto& command = m_ControllerBinds[y].second;
						command->Execute(m_Players[i]);
					}
					else if (IsPressed(static_cast<unsigned int>(key), static_cast<int>(i)))
					{
						auto& command = m_ControllerBinds[y].second;
						command->Execute(m_Players[i]);
					}
				}
			}

		}



		void KeyboardHandler()
		{

			const Uint8* keystate = SDL_GetKeyboardState(NULL);
			for (size_t i = 0; i < m_KeyBinds.size(); i++)
			{
				if (keystate[m_KeyBinds[i].button])
				{
					m_KeyBinds[i].command->Execute(m_Players[m_KeyBinds[i].inputID]);
				}
			}

		}




		bool KeyBoardEventhandler()
		{
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					return false;
				}
				if (e.type == SDL_KEYDOWN) {

				}
				if (e.type == SDL_MOUSEBUTTONDOWN) {

				}


				ImGui_ImplSDL2_ProcessEvent(&e);

			}

			return true;

		}



		void AddControllerBinding(InputManager::ControllerButton button, Command* command)
		{
			m_ControllerBinds.emplace_back(std::make_pair(button, command));
		}


		void AddKeyBinding(SDL_Scancode button, Command* command, int inputID)
		{
			KeyBind tempKeyBind{};
			tempKeyBind.button = button;
			tempKeyBind.command = command;
			tempKeyBind.inputID = inputID;

			m_KeyBinds.emplace_back(tempKeyBind);
		}






	};

	/////////////////////////////////////////////////
	/////////////////////////////////////////////////





	bool InputManager::ProcessInput()
	{
		return pImpl->ProcessInput();
	}

	bool InputManager::IsPressed(ControllerButton button, int ID) const
	{

		return pImpl->IsPressed(static_cast<unsigned int>(button), ID);
	}

	bool InputManager::IsDown(ControllerButton button, int ID) const
	{
		return pImpl->IsDownThisFrame(static_cast<unsigned int>(button), ID);
	}

	bool InputManager::IsUp(ControllerButton button, int ID) const
	{
		return pImpl->IsUpThisFrame(static_cast<unsigned int>(button), ID);
	}


	int InputManager::AssignController(GameObject* actor)
	{
		return pImpl->AssignController(actor);
	}

	InputManager::InputManager()
	{
		pImpl = new InputManagerImpl();
	}

	InputManager::~InputManager()
	{
		delete pImpl;
	}

	void InputManager::AddControllerBinding(ControllerButton button, Command* command)
	{
		pImpl->AddControllerBinding(button, command);
	}

	void InputManager::AddKeyBinding(SDL_Scancode button, Command* command, int inputID)
	{
		pImpl->AddKeyBinding(button, command, inputID);
	}


}