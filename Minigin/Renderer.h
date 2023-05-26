#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	


		SDL_Texture* m_backgroundTexture{};
		SDL_Texture* m_canvasTexture{};
		SDL_Texture* m_combinedTexture{};


	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, int x, int y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, int x, int y, int width, int height) const;

		void RenderTexture(SDL_Texture* texture, SDL_Rect* sourceRect, SDL_Rect* destRect, float angle, SDL_Point* center, SDL_RendererFlip flip) const;

		SDL_Renderer* GetSDLRenderer() const;

		SDL_Texture* GetCanvas() { return m_canvasTexture; }
		SDL_Texture* GetBackground() { return m_backgroundTexture; }


		bool SetBackgroundTexture(SDL_Texture* texture);

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

