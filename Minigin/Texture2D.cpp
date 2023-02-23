#include <SDL.h>
#include "Texture2D.h"
#include "Renderer.h"
#include <SDL_image.h>
#include <stdexcept>
#include <iostream>

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

glm::ivec2 dae::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture)
	: m_Texture{ nullptr },
	m_Width{},
	m_Height{}
{
	m_Texture = texture;
}

dae::Texture2D::Texture2D(std::string filePath)
	: m_Texture{nullptr},
	m_Width{},
	m_Height{}
{
	SDL_Texture* texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), filePath.c_str());

	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	//To get the image total width and height

	SDL_Surface* pLoadedSurface = IMG_Load(filePath.c_str());
	if (pLoadedSurface == nullptr)
	{
		std::cerr << "Texture::CreateFromImage, error when calling IMG_Load: " << SDL_GetError() << '\n';
		return;
	}

	m_Width = static_cast<float>(pLoadedSurface->w);
	m_Height = static_cast<float>(pLoadedSurface->h);

	// Free loaded surface
	SDL_FreeSurface(pLoadedSurface);

	m_Texture = static_cast<SDL_Texture*>(texture);
}
