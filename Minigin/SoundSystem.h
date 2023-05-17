#pragma once
#include <iostream>
#include <unordered_map>
#include "SDL_mixer.h"

#include <condition_variable>
#include <mutex>

#include <future>

namespace dae
{
	using sound_id = size_t;

	struct PlayMessage
	{
		sound_id id;
		float volume;
	};


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

		virtual void Update() = 0;
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

		void Update() override;
		void Play(const sound_id id, const float volume) override;
		void LoadSound(AudioClip* audioclip) override;
		void AddSound(const char* path, sound_id id = -1) override;
		void PlayMusic(const char* path) override;

		void SetMusicVolume(int volume);



	private:

		static const int MAX_PENDING = 16;

		static PlayMessage m_Pending[MAX_PENDING];
		static int m_NumPending;

		std::vector<AudioClip*> m_AudioClips;
		Mix_Music* m_BackgroundMusic;


		//threads


		std::vector<std::future<void>> m_LoadFutures;
		std::vector<std::future<void>> m_PlayFutures;

		void LoadSoundThread(AudioClip* audioClip, std::promise<void> promise);

		void LoadAndPlaySound(AudioClip* audioClip, float volume);

		void PlayAsync(AudioClip* audioClip, float volume);
		//void ProcessAudioClip(AudioClip* audioClip, float volume, std::promise<void> promise);
	};




	class LoggingSoundSystem final : public SoundSystem {
		SoundSystem* m_SoundSystem;
	public:
		LoggingSoundSystem(SoundSystem* ss) : m_SoundSystem(ss) {}
		~LoggingSoundSystem() { delete m_SoundSystem; }


		void Update() override;


		void Play(const sound_id id, const float volume) override;

		void LoadSound(AudioClip* audioclip) override;
		void AddSound(const char* path, sound_id id = -1) override;
		void PlayMusic(const char* path) override;
		void SetMusicVolume(int volume);
	};
}