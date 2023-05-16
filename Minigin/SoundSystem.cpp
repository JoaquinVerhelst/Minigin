#include "SoundSystem.h"
#include <SDL.h>

#include <chrono>
namespace dae
{

    SDL_SoundSystem::SDL_SoundSystem()
    {

        // Initialize SDL and SDL_mixer
        SDL_Init(SDL_INIT_AUDIO);
        Mix_Init(MIX_INIT_MP3);


        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);


        AddSound("../Data/Sound/Damage.wav");
        AddSound("../Data/Sound/Collect.wav");
        AddSound("../Data/Sound/GoldBreaking.wav");
        PlayMusic("../Data/Sound/LadyMaria.mp3");

        SetMusicVolume(32);
    }

    SDL_SoundSystem::~SDL_SoundSystem()
    {
        for (auto* audio : m_AudioClips) {
            Mix_FreeChunk(audio->audio);
            delete audio;
        }


        Mix_HaltMusic();
        Mix_FreeMusic(m_BackgroundMusic);
        Mix_CloseAudio();
    }

    void SDL_SoundSystem::Play(const sound_id id, const float volume)
    {
        
        auto start = std::chrono::high_resolution_clock::now();


        if (m_AudioClips[id]->audio == nullptr)
        {
            std::cout << "Loading..." << '\n';
            LoadSound(m_AudioClips[id]);
        }



 
        if (!m_AudioClips[id]->audio)
        {
            std::cerr << "Audio sound was nullptr" << std::endl;
            return;
        }


        Mix_VolumeChunk(m_AudioClips[id]->audio, static_cast<int>(volume * MIX_MAX_VOLUME));



        if (Mix_PlayChannel(-1, m_AudioClips[id]->audio, 0) == -1)
        {
            std::cerr << "Could not play sound" << std::endl;
            return;
        }



        auto end = std::chrono::high_resolution_clock::now();


        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "play took: " << elapsed << "ms\n";

    }




    void SDL_SoundSystem::LoadSound(AudioClip* audioclip)
    {

        Mix_Chunk* sound = Mix_LoadWAV(audioclip->pathFile);
        if (!sound) {
            std::cerr << "Error: Could not load sound ID: " << audioclip->id << "with the given path: " << audioclip->pathFile << std::endl;
            return;
        }

        audioclip->audio = sound;

    }

    void SDL_SoundSystem::AddSound(const char* path, sound_id id)
    {

        AudioClip* audioClip = new AudioClip();
        audioClip->audio = nullptr;
        audioClip->pathFile = path;

        if (id == -1)
        {
            audioClip->id = m_AudioClips.size();
            m_AudioClips.emplace_back(audioClip);
        }
        else if ( id < m_AudioClips.size())
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



    void SDL_SoundSystem::PlayMusic(const char* path)
    {

        m_BackgroundMusic = Mix_LoadMUS(path);
        if (!m_BackgroundMusic) {
            printf("Mix_LoadMUS failed: %s\n", Mix_GetError());
            return;
        }

        Mix_PlayMusic(m_BackgroundMusic, -1); // loop the music indefinitely


    }

    void SDL_SoundSystem::SetMusicVolume(int volume)
    {

        if (volume < 0) {
            volume = 0;
        }
        else if (volume > 128) {
            volume = 128;
        }


        Mix_VolumeMusic(volume);
    }




    void LoggingSoundSystem::Play(const sound_id id, const float volume)
    {

        m_SoundSystem->Play(id, volume);

        std::cout << "playing " << id << " at volume " << volume << '\n';

    }

}