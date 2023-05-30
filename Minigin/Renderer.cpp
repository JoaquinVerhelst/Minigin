#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "World.h"
#include "SDL_image.h"

//#include "ImGui/imgui.h"
//#include "ImGui/imgui_impl_opengl2.h"
//#include "ImGui/imgui_impl_sdl2.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	int width{};
	int height{};

	SDL_GetWindowSize(window, &width, &height);


	m_combinedTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(m_combinedTexture, SDL_BLENDMODE_BLEND);


	m_backgroundTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(m_backgroundTexture, SDL_BLENDMODE_BLEND);


	m_canvasTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(m_canvasTexture, SDL_BLENDMODE_BLEND);



	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	//ImGui_ImplOpenGL2_Init();

}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);


	//NEVER TOUCH THIS AGAIN 

	SDL_SetRenderTarget(m_renderer, m_combinedTexture);

	SDL_RenderClear(m_renderer);


	SDL_RenderCopy(m_renderer, m_backgroundTexture, nullptr, nullptr);

	SDL_RenderCopy(m_renderer, m_canvasTexture, nullptr, nullptr);
	

	SDL_SetRenderTarget(m_renderer, nullptr);


	SDL_RenderCopy(m_renderer, m_combinedTexture, nullptr, nullptr);

	World::GetInstance().Render();
	SceneManager::GetInstance().Render();


	SDL_RenderPresent(m_renderer);

	



	/*ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());*/

	//SDL_RenderPresent(m_renderer);

}

void dae::Renderer::Destroy()
{
	//ImGui_ImplOpenGL2_Shutdown();
	//ImGui_ImplSDL2_Shutdown();
	//ImGui::DestroyContext();


	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, int x, int y) const
{
	SDL_Rect dst{};
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, int x, int y, int width, int height) const
{
	SDL_Rect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

//A more Advanced render texture, but not tested
void dae::Renderer::RenderTexture(SDL_Texture* texture, SDL_Rect* sourceRect, SDL_Rect* destRect, float angle, SDL_Point* center, SDL_RendererFlip flip) const
{
	SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_RenderCopyEx(GetSDLRenderer(), texture, sourceRect, destRect, angle, center, flip);
}

inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }




bool dae::Renderer::SetBackgroundTexture(SDL_Texture* texture)
{

	m_backgroundTexture = texture;

	SDL_SetTextureBlendMode(m_backgroundTexture, SDL_BLENDMODE_BLEND);


	return true;
}
