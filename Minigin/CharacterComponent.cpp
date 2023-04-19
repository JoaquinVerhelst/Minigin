#include "CharacterComponent.h"
#include "Command.h"
#include "InputManager.h"
#include "Time.h"
#include "State.h"


namespace dae
{

	CharacterComponent::CharacterComponent(GameObject* owner, float walkSpeed, bool isControlledByPlayer)
		: dae::Component(owner)
		, m_WalkSpeed{ walkSpeed }
		, m_MaxCellsX{ 0 }
		, m_MaxCellsY{0}
		, m_CurrentState{ new IdleState }
		, m_InputID{0}
	{
		auto& input = InputManager::GetInstance();

		if (isControlledByPlayer)
		{
			m_InputID = input.AssignController(GetOwner());
		}
		else
		{
			m_InputID = -1;
		}


		//SDL_Window* window = SDL_GL_GetCurrentWindow();
		//int* width{};
		//int* height{};

		//SDL_GetWindowSize(window, width, height);


		//m_MaxCellsX = *width / m_GridCellSize;
		//m_MaxCellsY = *height / m_GridCellSize;


		//delete window;
		//delete width;
		//delete height;
	}

	CharacterComponent::~CharacterComponent()
	{
		delete m_CurrentState;
		m_CurrentState = nullptr;
	}

	void CharacterComponent::Update()
	{
		//m_CurrentState->Update(this);

		//auto pos = GetOwner()->GetPosition().GetPosition();


		//int currentCellX = pos.x / m_GridCellSize;
		//int currentCellY = pos.y / m_GridCellSize;





	}


	void CharacterComponent::Init()
	{
	}



	void CharacterComponent::Walk(int direction, int inputType, StateType newStateType)
	{


		if (!m_CurrentState->HandleInput(this, newStateType, inputType))
			return;



		glm::vec3 currentPos = GetOwner()->GetPosition().GetPosition();

		auto& time = Time::GetInstance();

		switch (direction)
		{
		case 0:
			GetOwner()->SetPosition(currentPos.x, currentPos.y - m_WalkSpeed * time.GetDeltaTime());
			break;
		case 1:
			GetOwner()->SetPosition(currentPos.x, currentPos.y + m_WalkSpeed * time.GetDeltaTime());
			break;
		case 2:
			GetOwner()->SetPosition(currentPos.x + m_WalkSpeed * time.GetDeltaTime(), currentPos.y);
			break;
		case 3:
			GetOwner()->SetPosition(currentPos.x - m_WalkSpeed * time.GetDeltaTime(), currentPos.y);
			break;
		}

	}

	void CharacterComponent::SetState(State* newState)
	{
		if (m_CurrentState != nullptr)
		{
			delete m_CurrentState;
			m_CurrentState = nullptr;
		}

		m_CurrentState = newState;
	}


}