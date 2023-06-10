#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif






#include "Minigin.h"

#include "JsonManager.h"

#include "World.h"


using namespace dae;

void load()
{

	JsonManager::GetInstance().LoadLevelsFromJson("../Data/JSON/DiggerLevels.json");

	World::GetInstance().ResetAndLoadWorld(0);
}

int main(int, char* []) {
	dae::Minigin engine("");
	engine.Run(load);
	return 0;
}

