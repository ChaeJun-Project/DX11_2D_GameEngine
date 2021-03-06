#pragma once
#include "IResource.h"

class AudioClip final : public IResource
{
public:
    AudioClip(const std::string& audio_clip_resource_name);
    explicit AudioClip(const AudioClip& origin);
    ~AudioClip();

public:
    void Play();
    void Pause();
    void Resume();
    void Stop();

public:
    void SetLoop(const bool& is_loop);
    void SetVolume(const float& volume);
    void SetAudioType(const AudioModeType& audio_mode_type);

private:
    void CreateAudio(const std::string& audio_clip_path);

public:
    bool LoadFromFile(const std::string& audio_clip_path) override;
   
public:
    CLONE(AudioClip);

private:
    FMOD::Sound* m_p_sound = nullptr;
    FMOD::Channel* m_p_channel = nullptr;

    float m_min_distance = 1.0f;
    float m_max_distance = 10000.0f;

    int m_loop_mode  = 0;
    int m_audio_mode = 0;

    friend class AudioSource;
};

