#include "SoundSystem.h"

#include <SDL.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "SDL_mixer.h"
#include <condition_variable>
#include <mutex>
#include <future>


namespace dae
{




    class SDL_SoundSystem::SDL_SoundSystemImpl
    {
    private:

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


        static const int MAX_PENDING = 16;

        static PlayMessage m_Pending[MAX_PENDING];
        static int m_NumPending;

        std::vector<AudioClip*> m_AudioClips;
        Mix_Music* m_BackgroundMusic;



        //threads


        std::vector<std::future<void>> m_LoadFutures;
        std::vector<std::future<void>> m_PlayFutures;



        void LoadAndPlaySound(AudioClip* audioClip, float volume);

        void PlaySound(AudioClip* audioClip, float volume);


    public:

        SDL_SoundSystemImpl();
        ~SDL_SoundSystemImpl();

        void Update();

        void Play(const sound_id id, const float volume);
        void LoadSound(const sound_id id);
        void AddSound(const char* path, const sound_id id = -1);
        void PlayMusic(const char* path);

        void SetMusicVolume(int volume);

    };



    SDL_SoundSystem::SDL_SoundSystemImpl::PlayMessage SDL_SoundSystem::SDL_SoundSystemImpl::m_Pending[MAX_PENDING];
    int SDL_SoundSystem::SDL_SoundSystemImpl::m_NumPending = 0;

    // SDL Sound pImpl System functions------------------------------------------------------
    // -----------------------------------------------------------------------


    SDL_SoundSystem::SDL_SoundSystemImpl::SDL_SoundSystemImpl()
    {
        
        // Initialize SDL and SDL_mixer
        SDL_Init(SDL_INIT_AUDIO);
        Mix_Init(MIX_INIT_MP3);


        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);


        AddSound("../Data/Sound/Damage.wav");
        AddSound("../Data/Sound/Collect.wav");
        AddSound("../Data/Sound/GoldBreaking.wav");
        //PlayMusic("../Data/Sound/LadyMaria.mp3");

        //SetMusicVolume(32);
        
    }

    SDL_SoundSystem::SDL_SoundSystemImpl::~SDL_SoundSystemImpl()
    {
        for (auto* audio : m_AudioClips) {
            Mix_FreeChunk(audio->audio);
            delete audio;
        }


        Mix_HaltMusic();
        Mix_FreeMusic(m_BackgroundMusic);
        Mix_CloseAudio();
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::Update()
    {
        for (size_t i = 0; i < m_NumPending; ++i)
        {
            auto start = std::chrono::high_resolution_clock::now();

            AudioClip* audioClip = m_AudioClips[m_Pending[i].id];

            if (audioClip->audio == nullptr)
            {
                std::cout << "Loading audio...\n";

                // Create a promise and get its future
                std::promise<void> promise;
                std::future<void> future = promise.get_future();

                // Create a separate thread to load and play the sound
                std::thread thread([this, audioClip, volume = m_Pending[i].volume, promise = std::move(promise)]() mutable {
                    LoadAndPlaySound(audioClip, volume);
                    promise.set_value();
                    });

                // Detach the thread to run independently
                thread.detach();
            }
            else
            {
                std::cout << "Sound is already loaded. Playing...\n";

                // Create a separate thread to play the sound
                std::thread thread([this, audioClip, volume = m_Pending[i].volume]() {
                    PlaySound(audioClip, volume);
                    });

                // Detach the thread to run independently
                thread.detach();
            }


            auto end = std::chrono::high_resolution_clock::now();


            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            std::cout << "play took: " << elapsed << "ms\n";
        }

        m_NumPending = 0;
    }

    void SDL_SoundSystem::SDL_SoundSystemImpl::Play(const sound_id id, const float volume)
    {
        if (m_NumPending >= MAX_PENDING)
        {
            std::cout << "Maximum pending sounds. Cannot play ID: " << id << '\n';
            return;
        }

        m_Pending[m_NumPending].id = id;
        m_Pending[m_NumPending].volume = volume;

        ++m_NumPending;
    }

    void dae::SDL_SoundSystem::SDL_SoundSystemImpl::LoadAndPlaySound(AudioClip* audioClip, float volume)
    {
        std::cout << "Loading audio...\n";
        LoadSound(audioClip->id);
        PlaySound(audioClip, volume);
    }

    void dae::SDL_SoundSystem::SDL_SoundSystemImpl::LoadSound(const sound_id id)
    {
        auto* audioclip = m_AudioClips[id];

        Mix_Chunk* sound = Mix_LoadWAV(audioclip->pathFile);
        if (!sound) {
            std::cerr << "Error: Could not load sound ID: " << audioclip->id << "with the given path: " << audioclip->pathFile << std::endl;
            return;
        }

        audioclip->audio = sound;
    }

    void dae::SDL_SoundSystem::SDL_SoundSystemImpl::PlaySound(AudioClip* audioClip, float volume)
    {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        Mix_VolumeChunk(audioClip->audio, static_cast<int>(volume * MIX_MAX_VOLUME));

        if (Mix_PlayChannel(-1, audioClip->audio, 0) == -1)
        {
            std::cout << "Could not play sound" << '\n';
            return;
        }
    }

    void dae::SDL_SoundSystem::SDL_SoundSystemImpl::AddSound(const char* path, sound_id id)
    {

        AudioClip* audioClip = new AudioClip();
        audioClip->audio = nullptr;
        audioClip->pathFile = path;

        if (id == -1)
        {
            audioClip->id = m_AudioClips.size();
            m_AudioClips.emplace_back(audioClip);
        }
        else if (id < m_AudioClips.size())
        {
            audioClip->id = id;
            delete m_AudioClips[id];
            m_AudioClips[id] = audioClip;
        }
        else
        {
            std::cout << "Failed to add audio to ID: " << id << "with the given path: " << path << '\n';
        }

    }


    void dae::SDL_SoundSystem::SDL_SoundSystemImpl::PlayMusic(const char* path)
    {
        m_BackgroundMusic = Mix_LoadMUS(path);
        if (!m_BackgroundMusic) {
            printf("Mix_LoadMUS failed: %s\n", Mix_GetError());
            return;
        }

        Mix_PlayMusic(m_BackgroundMusic, -1); // loop the music indefinitely
    }

    void dae::SDL_SoundSystem::SDL_SoundSystemImpl::SetMusicVolume(int volume)
    {
        if (volume < 0) {
            volume = 0;
        }
        else if (volume > 128) {
            volume = 128;
        }


        Mix_VolumeMusic(volume);
    }





    // SDL Sound System ------------------------------------------------------
    // -----------------------------------------------------------------------


    SDL_SoundSystem::SDL_SoundSystem()
    {
        pImpl = new SDL_SoundSystemImpl();
    }

    SDL_SoundSystem::~SDL_SoundSystem()
    {
        delete pImpl;
    }

    void SDL_SoundSystem::Update()
    {
        pImpl->Update();
    }

    void SDL_SoundSystem::Play(const sound_id id, const float volume)
    {
        pImpl->Play(id, volume);
    }

    void SDL_SoundSystem::LoadSound(sound_id id)
    {
        pImpl->LoadSound(id);
    }

    void SDL_SoundSystem::AddSound(const char* path, sound_id id)
    {
        pImpl->AddSound(path, id);
    }

    void SDL_SoundSystem::PlayMusic(const char* path)
    {
        pImpl->PlayMusic(path);
    }

    void SDL_SoundSystem::SetMusicVolume(int volume)
    {
        pImpl->SetMusicVolume(volume);
    }




    // Logging System ------------------------------------------------------
    // ---------------------------------------------------------------------


    void LoggingSoundSystem::Update()
    {
        m_SoundSystem->Update();
    }

    void LoggingSoundSystem::Play(const sound_id id, const float volume)
    {
        //auto start = std::chrono::high_resolution_clock::now();

        m_SoundSystem->Play(id, volume);

        //auto end = std::chrono::high_resolution_clock::now();
        //auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        //std::cout << "playing " << id << " at volume " << volume << '\n';
        //std::cout << "playing the sound took: " << elapsed << "ms\n";


    }

    void LoggingSoundSystem::LoadSound(sound_id id)
    {

        m_SoundSystem->LoadSound(id);
    }

    void LoggingSoundSystem::AddSound(const char* path, sound_id id)
    {
        m_SoundSystem->AddSound(path, id);
    }

    void LoggingSoundSystem::PlayMusic(const char* path)
    {
        m_SoundSystem->PlayMusic(path);
    }

    void LoggingSoundSystem::SetMusicVolume(int volume)
    {
        m_SoundSystem->SetMusicVolume(volume);
    }



}