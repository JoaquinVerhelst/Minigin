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


#include "SoundServiceLocator.h"
#include "SoundSystem.h"


SDL_Window* g_window{};






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

	SoundServiceLocator::RegisterSoundSystem(new LoggingSoundSystem( new SDL_SoundSystem()));




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

	std::cout << "Player1 Controls: Controller 1 DPad and WASD" << '\n';
	std::cout << "Player2 Controls: Controller 2 DPad and arrow keys" << '\n';

	std::cout << "Add Score =  ButtonA" << '\n';

	std::cout << "remove Health =  ButtonB" << '\n';

	//Temporary way for initializition  TODO: Change The Command -> no objects

	auto* walkUp = new VerticalWalkCommand(1);
	auto* walkDown = new VerticalWalkCommand(0);
	auto* walkRight = new HorizontalWalkCommand(0);
	auto* walkLeft = new HorizontalWalkCommand(1);
	auto* Selfdamage = new SelfDamageCommand();
	auto* addScore = new AddScoreCommand();

	input.AddControllerBinding(InputManager::ControllerButton::DPadUp, walkUp, Command::InputType::Pressed);
	input.AddControllerBinding(InputManager::ControllerButton::DPadDown, walkDown, Command::InputType::Pressed);
	input.AddControllerBinding(InputManager::ControllerButton::DPadRight, walkRight, Command::InputType::Pressed);
	input.AddControllerBinding(InputManager::ControllerButton::DPadLeft, walkLeft, Command::InputType::Pressed);

	input.AddControllerBinding(InputManager::ControllerButton::ButtonB, Selfdamage, Command::InputType::Up);

	input.AddControllerBinding(InputManager::ControllerButton::ButtonA, addScore, Command::InputType::Up);


	input.AddKeyBinding(SDL_SCANCODE_W, walkUp, 0);
	input.AddKeyBinding(SDL_SCANCODE_S, walkDown, 0);
	input.AddKeyBinding(SDL_SCANCODE_A, walkLeft, 0);
	input.AddKeyBinding(SDL_SCANCODE_D, walkRight, 0);

	input.AddKeyBinding(SDL_SCANCODE_UP, walkUp, 1);
	input.AddKeyBinding(SDL_SCANCODE_DOWN, walkDown, 1);
	input.AddKeyBinding(SDL_SCANCODE_LEFT, walkLeft, 1);
	input.AddKeyBinding(SDL_SCANCODE_RIGHT, walkRight, 1);



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





	delete walkUp;
	delete walkDown;
	delete walkRight;
	delete walkLeft;
	delete Selfdamage;
	delete addScore;

}

void dae::Minigin::SetUpInput()
{



}
