#include "MenuComponent.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Scene.h"

#include "World.h"
#include <iostream>
#include "SDL.h"

namespace dae
{
	MenuComponent::MenuComponent(GameObject* owner)
		:  Component(owner)
		, m_SinglePlayerButton{  }
		, m_CoopButton{ }
		, m_VersusButton{ }
		, m_QuitButton{ }
		, m_HighScoreButton{ }
		, m_MousePos{}
		, m_SelectedButton{}
	{




	}

	MenuComponent::~MenuComponent()
	{
		delete m_SinglePlayerButton.texture;
		delete m_CoopButton.texture;
		delete m_VersusButton.texture;
		delete m_QuitButton.texture;
		delete m_HighScoreButton.texture;
	}


	void MenuComponent::Render() const
	{

		//const SDL_Rect rect = SDL_Rect{ m_MousePos.x, m_MousePos.y, 20, 20 };
		//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 255);
		//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);

		Renderer::GetInstance().RenderTexture(*m_SinglePlayerButton.texture, m_SinglePlayerButton.shape.x, m_SinglePlayerButton.shape.y, m_SinglePlayerButton.shape.w, m_SinglePlayerButton.shape.h);
		Renderer::GetInstance().RenderTexture(*m_CoopButton.texture, m_CoopButton.shape.x, m_CoopButton.shape.y, m_CoopButton.shape.w, m_CoopButton.shape.h);
		Renderer::GetInstance().RenderTexture(*m_VersusButton.texture, m_VersusButton.shape.x, m_VersusButton.shape.y, m_VersusButton.shape.w, m_VersusButton.shape.h);
		Renderer::GetInstance().RenderTexture(*m_QuitButton.texture, m_QuitButton.shape.x, m_QuitButton.shape.y, m_QuitButton.shape.w, m_QuitButton.shape.h);

		Renderer::GetInstance().RenderTexture(*m_HighScoreButton.texture, m_HighScoreButton.shape.x, m_HighScoreButton.shape.y, m_HighScoreButton.shape.w, m_HighScoreButton.shape.h);

	}

	void MenuComponent::Update()
	{
		ProcessMouseEvents();
	}

	void MenuComponent::Init()
	{
		m_SinglePlayerButton.shape = SDL_Rect{ 400, 250, 250, 70 };
		m_SinglePlayerButton.texture = new Texture2D{ "../Data/Sprites/SinglePlayerButton.png" };

		m_CoopButton.shape = SDL_Rect{ 400, 340, 250, 70 };
		m_CoopButton.texture = new Texture2D{ "../Data/Sprites/CoopButton.png" };

		m_VersusButton.shape = SDL_Rect{ 400, 430, 250, 70 };
		m_VersusButton.texture = new Texture2D{ "../Data/Sprites/VersusButton.png" };

		m_QuitButton.shape = SDL_Rect{ 400, 520, 250, 70 };
		m_QuitButton.texture = new Texture2D{ "../Data/Sprites/QuitButton.png" };

		m_HighScoreButton.shape = SDL_Rect{ 400, 610, 250, 70 };
		m_HighScoreButton.texture = new Texture2D{ "../Data/Sprites/HighScoreButton.png" };

	}

	void MenuComponent::ProcessMouseEvents()
	{

		auto pos = InputManager::GetInstance().GetMouseLocation();

		if ( pos != glm::ivec2(0,0))
		{
			m_MousePos = pos;
		}
		
		if (InputManager::GetInstance().GetMouseAction(SDL_MOUSEBUTTONUP))
		{
			HitButtons();
		}
	}

	void MenuComponent::HitButtons()
	{

		if (IsPointInRect(m_MousePos, m_SinglePlayerButton.shape))
		{
			std::cout << "Singleplayer";
			SceneManager::GetInstance().NextScene();
			World::GetInstance().SetGameMode(World::GameModeTypes::SinglePlayer);

		}
		else if (IsPointInRect(m_MousePos, m_CoopButton.shape))
		{
			std::cout << "Coop";
			SceneManager::GetInstance().NextScene();
			World::GetInstance().SetGameMode(World::GameModeTypes::Coop);

		}
		else if (IsPointInRect(m_MousePos, m_VersusButton.shape))
		{
			std::cout << "Versus";
			SceneManager::GetInstance().NextScene();
			World::GetInstance().SetGameMode(World::GameModeTypes::Versus);
		}
		else if (IsPointInRect(m_MousePos, m_HighScoreButton.shape))
		{
			std::cout << "HIGHSCORE";
			SceneManager::GetInstance().NextSceneByIndex(4);
		}
		else if (IsPointInRect(m_MousePos, m_QuitButton.shape))
		{
			std::cout << "QUIT";
			InputManager::GetInstance().QuitGame(true);
		}
	}

	bool MenuComponent::IsPointInRect(const glm::ivec2& p, const SDL_Rect& r)
	{
		return (p.x >= r.x &&
			p.x <= r.x + r.w &&
			p.y >= r.y &&
			p.y <= r.y + r.h);
	}


}