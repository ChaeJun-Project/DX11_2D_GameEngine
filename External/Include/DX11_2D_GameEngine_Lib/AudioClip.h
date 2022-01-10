#pragma once
#include "IResource.h"

class AudioClip final : public IResource
{
public:
    AudioClip(const std::string resource_name);
    ~AudioClip();

public:
    void Play();
    void Pause();
    void Stop();

public:
    void SetLoop(const bool& is_loop);
    void SetVolume(const float& volume);

private:
    void CreateAudio(const std::string& audio_clip_path);

public:
    void LoadFromFile(const std::string& audio_clip_path) override;
   
private:
    FMOD::Sound* m_p_sound = nullptr;
    FMOD::Channel* m_p_channel = nullptr;

    float m_min_distance = 1.0f;
    float m_max_distance = 10000.0f;

    int m_loop_mode  = 0;

    friend class AudioSource;
};

