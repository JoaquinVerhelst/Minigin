#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>
#include <future>
#include <thread>


namespace dae
{
	using sound_id = size_t;







	class SoundSystem
	{



	public:
		virtual ~SoundSystem() = default;

		virtual void Update() = 0;
		virtual void Play(const sound_id id, const float volume) = 0;
		virtual void LoadSound(sound_id id) = 0;
		virtual void AddSound(const char* path, sound_id id = -1) = 0;
		virtual void PlayMusic(const char* path) = 0;
		virtual void SetMusicVolume(int volume) = 0;
	};




	class SDL_SoundSystem final : public SoundSystem
	{


		class SDL_SoundSystemImpl;
		SDL_SoundSystemImpl* pImpl;


	public:

		SDL_SoundSystem();
		~SDL_SoundSystem();

		void Update() override;
		void Play(const sound_id id, const float volume) override;
		void LoadSound(sound_id id) override;
		void AddSound(const char* path, sound_id id = -1) override;
		void PlayMusic(const char* path) override;

		void SetMusicVolume(int volume);



	private:



	};


	class LoggingSoundSystem final : public SoundSystem {
		SoundSystem* m_SoundSystem;
	public:
		LoggingSoundSystem(SoundSystem* ss) : m_SoundSystem(ss) {}
		~LoggingSoundSystem() { delete m_SoundSystem; }


		void Update() override;


		void Play(const sound_id id, const float volume) override;

		void LoadSound(sound_id id) override;
		void AddSound(const char* path, sound_id id = -1) override;
		void PlayMusic(const char* path) override;
		void SetMusicVolume(int volume);
	};

}