#pragma once
#include "SoundSystem.h"

class SoundServiceLocator final
{
	static SoundSystem* _ss_instance;
public:
	static SoundSystem& get_sound_system() { return *_ss_instance; }
	static void register_sound_system(SoundSystem* ss) { _ss_instance = ss; }
};