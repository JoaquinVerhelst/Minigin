#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>


#include "InputManager.h"
#include "ImGui/imgui_impl_sdl2.h"

#include <vector>
#include "SceneManager.h"
#include <iostream>







namespace dae
{

	class InputManager::InputManagerImpl
	{

	private:



		struct KeyBind
		{
			std::shared_ptr<Command> command;
			SDL_Scancode button;
			int inputID;
		};

		struct ControllerBind
		{
			std::shared_ptr<Command> command;
			ControllerButton button;
			Command::InputType inputType;
		};

		XINPUT_STATE* m_PreviousStates;
		XINPUT_STATE* m_CurrentStates;

		WORD* m_ButtonsPressedThisFrame;
		WORD* m_ButtonsReleasedThisFrame;

		std::vector<GameObject*> m_Players{};

		std::vector<ControllerBind> m_ControllerBinds;

		std::vector<KeyBind> m_KeyBinds;





		//static const int MAX_PENDING = 16;

		//static KeyBind m_Pending[MAX_PENDING];
		//static int m_NumPending;


		bool m_QuitGame = false;

	public:


		InputManagerImpl()
			:m_CurrentStates(new XINPUT_STATE[XUSER_MAX_COUNT]), m_PreviousStates(new XINPUT_STATE[XUSER_MAX_COUNT])
			, m_ButtonsPressedThisFrame(new WORD[XUSER_MAX_COUNT]), m_ButtonsReleasedThisFrame(new WORD[XUSER_MAX_COUNT])
		{
		}

		bool ProcessInput()
		{

			//Controllers 

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


			//Input bindings check
			ControllersHandler();
			KeyboardHandler();




			return KeyBoardEventHandler();
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

	


		void ControllersHandler()
		{

			for (size_t i = 0; i < m_Players.size(); ++i)
			{
				for (size_t y = 0; y < m_ControllerBinds.size(); ++y)
				{

					const auto& key = m_ControllerBinds[y].button;
	
					if (IsPressed(static_cast<unsigned int>(key), static_cast<int>(i)))
					{
						auto& command = m_ControllerBinds[y].command;
						command->Execute(m_Players[i], Command::InputType::Pressed);
					}

					else if (IsUpThisFrame(static_cast<unsigned int>(key), static_cast<int>(i)))
					{
						auto& command = m_ControllerBinds[y].command;
						command->Execute(m_Players[i], Command::InputType::Up);
					}

					else if (IsDownThisFrame(static_cast<unsigned int>(key), static_cast<int>(i)))
					{
						auto& command = m_ControllerBinds[y].command;
						command->Execute(m_Players[i], Command::InputType::Down);
					}

				}
			}

		}



		void KeyboardHandler()
		{

			const Uint8* keystate = SDL_GetKeyboardState(NULL);
			for (size_t i = 0; i < m_KeyBinds.size(); i++)
			{

				if (m_KeyBinds[i].inputID <= static_cast<int>(m_Players.size() - 1))
				{
					if (keystate[m_KeyBinds[i].button])
					{
						m_KeyBinds[i].command->Execute(m_Players[m_KeyBinds[i].inputID], Command::InputType::Pressed);
					}
				}

			}
		}




		bool KeyBoardEventHandler()
		{
			SDL_Event e;

			while (SDL_PollEvent(&e) != 0)
			{
				GetFrameEvents().emplace_back(e);
			}

			for (size_t i = 0; i < GetFrameEvents().size(); i++)
			{
				if (GetFrameEvents()[i].type == SDL_QUIT || m_QuitGame) {
					return false;
				}
			}
			
			for (size_t j = 0; j < m_KeyBinds.size(); j++)
			{
				if (m_KeyBinds[j].inputID == -1 && IsKeyBoardKey(m_KeyBinds[j].button, SDL_KEYUP))
				{
					m_KeyBinds[j].command->Execute(nullptr, Command::InputType::Up);
				}
				else if (m_KeyBinds[j].inputID <= static_cast<int>(m_Players.size() - 1) && IsKeyBoardKey(m_KeyBinds[j].button, SDL_KEYUP))
					m_KeyBinds[j].command->Execute(m_Players[m_KeyBinds[j].inputID], Command::InputType::Up);

			}

			return true;

		}



		void AddControllerBinding(InputManager::ControllerButton button, std::shared_ptr<Command> command, Command::InputType inputType)
		{
			ControllerBind tempControllerBind{};
			tempControllerBind.button = button;
			tempControllerBind.command = command;
			tempControllerBind.inputType = inputType;

			m_ControllerBinds.emplace_back(tempControllerBind);

		}


		void AddKeyBinding(SDL_Scancode button, std::shared_ptr<Command> command, int inputID)
		{
			KeyBind tempKeyBind{};
			tempKeyBind.button = button;
			tempKeyBind.command = command;
			tempKeyBind.inputID = inputID;
			m_KeyBinds.emplace_back(tempKeyBind);	
		}



		bool IsKeyBoardKey(SDL_KeyCode key, SDL_EventType type )
		{
			for (size_t i = 0; i < GetFrameEvents().size(); i++)
			{
				if (GetFrameEvents()[i].type == static_cast<unsigned>(type))
				{
					if (GetFrameEvents()[i].key.keysym.sym == key)
					{
						return true;
					}
				}
			}

			return false;
		}

		bool IsKeyBoardKey(SDL_Scancode key, SDL_EventType type)
		{
			for (size_t i = 0; i < GetFrameEvents().size(); i++)
			{
				if (GetFrameEvents()[i].type == static_cast<unsigned>(type))
				{
					if (GetFrameEvents()[i].key.keysym.scancode == key)
					{
						return true;
					}
				}
			}

			return false;
		}

		glm::ivec2 GetMouseLocation()
		{

			auto e = GetFrameEvents();
			for (size_t i = 0; i < GetFrameEvents().size(); i++)
			{
				if (e[i].type == SDL_MOUSEMOTION)
				{
					return glm::ivec2{ (int)e[i].motion.x,(int)e[i].motion.y };
				}
			}

			return glm::ivec2{ 0,0 };
		}


		bool GetMouseAction(SDL_EventType type)
		{
			for (size_t i = 0; i < GetFrameEvents().size(); i++)
			{
				if (GetFrameEvents()[i].type == static_cast<unsigned>(type))
				{
					return true;
				}
			}

			return false;
		}


		std::vector<SDL_Event>& GetFrameEvents()
		{
			static std::vector<SDL_Event> frame_events;
			return frame_events;
		}

		void ClearFrameEvents()
		{
			GetFrameEvents().clear();
		}

	
		void QuitGame(bool quitgame)
		{
			m_QuitGame = quitgame;
		}


		std::string GetPressedKeyString()
		{

			for (size_t i = 0; i < GetFrameEvents().size(); i++)
			{
				if (GetFrameEvents()[i].type == SDL_KEYUP)
				{
					SDL_Keycode keyCode = GetFrameEvents()[i].key.keysym.sym;
					if ((keyCode >= SDLK_a && keyCode <= SDLK_z))
					{
						const char* keyName = SDL_GetKeyName(keyCode);
						return keyName;
					}
				}
			}
			

			return ""; // Return an empty string if no key is currently pressed
		}

		void ResetInput()
		{
			m_Players.clear();
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

	void InputManager::AddControllerBinding(ControllerButton button, std::shared_ptr<Command> command, Command::InputType inputType)
	{
		pImpl->AddControllerBinding(button, command, inputType);
	}

	void InputManager::AddKeyBinding(SDL_Scancode button, std::shared_ptr<Command> command, int inputID)
	{
		pImpl->AddKeyBinding(button, command, inputID);
	}

	void InputManager::ClearFrameEvents()
	{
		pImpl->ClearFrameEvents();
	}



	void InputManager::ResetInput()
	{
		pImpl->ResetInput();
	}

	bool InputManager::IsKeyBoardKey(SDL_Scancode key, SDL_EventType type)
	{
		return pImpl->IsKeyBoardKey(key, type);
	}

	bool InputManager::IsKeyBoardKey(SDL_KeyCode key, SDL_EventType type)
	{
		return pImpl->IsKeyBoardKey(key, type);
	}

	std::string InputManager::GetPressedKeyString()
	{
		return pImpl->GetPressedKeyString();
	}

	glm::ivec2 InputManager::GetMouseLocation()
	{
		return pImpl->GetMouseLocation();
	}

	bool InputManager::GetMouseAction(SDL_EventType type)
	{
		return pImpl->GetMouseAction( type);
	}

	void InputManager::QuitGame(bool quitgame)
	{
		pImpl->QuitGame(quitgame);
	}

}