#include "SoundServiceLocator.h"


namespace dae
{
	SoundSystem* SoundServiceLocator::_ss_instance = nullptr;


	void SoundServiceLocator::Destroy()
	{
		delete _ss_instance;
	}

	SoundSystem& SoundServiceLocator::GetSoundSysyem()
	{
		return *_ss_instance;
	}

	void SoundServiceLocator::RegisterSoundSystem(SoundSystem* ss)
	{
		_ss_instance = ss;
	}

}