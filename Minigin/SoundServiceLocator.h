#pragma once
#include "SoundSystem.h"


namespace dae
{


	class SoundServiceLocator final
	{
	private:
		static SoundSystem* _ss_instance;
	public:
		static void Destroy();
		static SoundSystem& GetSoundSysyem();
		static void RegisterSoundSystem(SoundSystem* ss);
	};

}