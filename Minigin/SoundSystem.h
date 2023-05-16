#pragma once
#include <iostream>
#include <unordered_map>
#include "SDL_mixer.h"



namespace dae
{
	using sound_id = size_t;





	struct AudioClip
	{
		sound_id id;
		Mix_Chunk* audio;
		const char* pathFile;
	};


	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const sound_id id, const float volume) = 0;
		virtual void LoadSound(AudioClip* audioclip) = 0;
		virtual void AddSound(const char* path, sound_id id = -1) = 0;
		virtual void PlayMusic(const char* path) = 0;
		virtual void SetMusicVolume(int volume) = 0;
	};




	class SDL_SoundSystem final : public SoundSystem
	{
	public:
		SDL_SoundSystem();
		~SDL_SoundSystem();

		void Play(const sound_id id, const float volume) override;
		void LoadSound(AudioClip* audioclip) override;
		void AddSound(const char* path, sound_id id = -1) override;
		void PlayMusic(const char* path) override;

		void SetMusicVolume(int volume);

	private:

		std::vector<AudioClip*> m_AudioClips;
		Mix_Music* m_BackgroundMusic;
	};


	class LoggingSoundSystem final : public SoundSystem {
		SoundSystem* m_SoundSystem;
	public:
		LoggingSoundSystem(SoundSystem* ss) : m_SoundSystem(ss) {}
		~LoggingSoundSystem() { delete m_SoundSystem; }
		void Play(const sound_id id, const float volume) override;
	};
}