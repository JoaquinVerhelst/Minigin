#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <chrono>
#include "Time.h"
#include "World.h"
#include <iostream>

#include "CharacterComponent.h"
#include "SoundServiceLocator.h"
#include "SoundSystem.h"


SDL_Window* g_window{};



#include "JsonManager.h"

#include "World.h"


void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}



	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1080,
		720,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);
	Time::GetInstance().Init();
	ResourceManager::GetInstance().Init(dataPath);
	World::GetInstance().Init(20, 14, g_window);

	SoundServiceLocator::RegisterSoundSystem( new SDL_SoundSystem());




}

dae::Minigin::~Minigin()
{
	SoundServiceLocator::Destroy();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();


	JsonManager::GetInstance().LoadLevelsFromJson("../Data/JSON/DiggerLevels.json");

	World::GetInstance().ResetAndLoadWorld(0);



	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& time = Time::GetInstance();
	auto& world = World::GetInstance();

	//Game loop
	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();
	float lag = 0.0f;

	float fixedTimeStep = 0.02f;

	std::cout << '\n';
	std::cout << '\n';

	std::cout << "Player1 Controls: " << '\n';
	std::cout << "Movement: Controller 1 DPad and WASD" << '\n';
	std::cout << "Use Specialty: space bar" << '\n';



	std::cout << "Player2 Controls: " << '\n'; 
	std::cout << "Movement: Controller 2 DPad and arrow keys" << '\n';
	std::cout << "Use Specialty: RCTRL" << '\n';


	std::cout << "KeyBoard: " << '\n';
	std::cout << "P key: mute sound" << '\n';
	std::cout << "F1 key: Next Level" << '\n';
	//Temporary way for initializition  TODO: Change The Command -> no objects



	input.AddControllerBinding(InputManager::ControllerButton::DPadUp, std::make_shared<VerticalWalkCommand>(Direction::Up), Command::InputType::Pressed);
	input.AddControllerBinding(InputManager::ControllerButton::DPadDown, std::make_shared<VerticalWalkCommand>(Direction::Down), Command::InputType::Pressed);
	input.AddControllerBinding(InputManager::ControllerButton::DPadRight, std::make_shared<HorizontalWalkCommand>(Direction::Right), Command::InputType::Pressed);
	input.AddControllerBinding(InputManager::ControllerButton::DPadLeft, std::make_shared<HorizontalWalkCommand>(Direction::Left), Command::InputType::Pressed);
	input.AddControllerBinding(InputManager::ControllerButton::ButtonA, std::make_shared<UseSpecialtyCommand>(), Command::InputType::Up);

	input.AddKeyBinding(SDL_SCANCODE_W, std::make_shared<VerticalWalkCommand>(Direction::Up), 0);
	input.AddKeyBinding(SDL_SCANCODE_S, std::make_shared<VerticalWalkCommand>(Direction::Down), 0);
	input.AddKeyBinding(SDL_SCANCODE_A, std::make_shared<HorizontalWalkCommand>(Direction::Left), 0);
	input.AddKeyBinding(SDL_SCANCODE_D, std::make_shared<HorizontalWalkCommand>(Direction::Right), 0);
	input.AddKeyBinding(SDL_SCANCODE_SPACE, std::make_shared<UseSpecialtyCommand>(), 0);

	input.AddKeyBinding(SDL_SCANCODE_P, std::make_shared<MuteCommand>(), -1);


	input.AddKeyBinding(SDL_SCANCODE_UP, std::make_shared<VerticalWalkCommand>(Direction::Up), 1);
	input.AddKeyBinding(SDL_SCANCODE_DOWN, std::make_shared<VerticalWalkCommand>(Direction::Down), 1);
	input.AddKeyBinding(SDL_SCANCODE_LEFT, std::make_shared<HorizontalWalkCommand>(Direction::Left), 1);
	input.AddKeyBinding(SDL_SCANCODE_RIGHT, std::make_shared<HorizontalWalkCommand>(Direction::Right), 1);
	input.AddKeyBinding(SDL_SCANCODE_RCTRL, std::make_shared<UseSpecialtyCommand>(), 1);



	while (doContinue)
	{
		time.Update();

		doContinue = input.ProcessInput();

		lag += time.GetDeltaTime();

		sceneManager.Update();
		world.Update();
		SoundServiceLocator::GetSoundSystem().Update();
		while (lag >= fixedTimeStep)
		{
			sceneManager.FixedUpdate();
			lag -= fixedTimeStep;
		}


		renderer.Render();

		input.GetInstance().ClearFrameEvents();

	}


}

void dae::Minigin::SetUpInput()
{



}
