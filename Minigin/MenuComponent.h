#pragma once

#include "Texture2D.h"
#include "Component.h"
#include "SDL.h"
#include "GameObject.h"

namespace dae
{
	struct Button
	{
		Texture2D* texture;
		SDL_Rect shape;
	};


	class MenuComponent final: public Component
	{
	public:
		MenuComponent(GameObject* owner);
		~MenuComponent();

		MenuComponent(const MenuComponent& other) = delete;
		MenuComponent& operator=(const MenuComponent& other) = delete;
		MenuComponent(MenuComponent&& other) = delete;
		MenuComponent& operator=(MenuComponent&& other) = delete;


		void Update() override;
		void Init() override;
		void Render() const override;
		bool IsRenderer() override { return true; }


		void HitButtons();
		//void HoverButtons();
		void ProcessMouseEvents();


		//void ProcesControllers(std::shared_ptr<InputManager> input);

		bool IsPointInRect(const glm::ivec2& p, const SDL_Rect& r);


		//int GetSceneID() { return m_SceneID; }


	private:


		Button m_SinglePlayerButton;
		Button m_CoopButton;
		Button m_VersusButton;
		Button m_QuitButton;
		Button m_HighScoreButton;



		glm::ivec2 m_MousePos;


		//0 == singleplayer, 1 == coop, 2 == versus
		int m_SelectedButton;

	};


}