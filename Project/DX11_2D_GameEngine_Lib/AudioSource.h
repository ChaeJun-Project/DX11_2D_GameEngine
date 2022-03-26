#pragma once
#include "IComponent.h"

class AudioClip;

class AudioSource final : public IComponent
{
public:
	AudioSource();
	explicit AudioSource(const AudioSource& origin);
	~AudioSource();

public:
    void Start() override;
	void FinalUpdate() override; //최종 업데이트 => 오브젝트가 움직이면 안됨

public:
	void Play();
	void Pause();
	void Resume();
	void Stop();

public:
    //Audio Clip
	void AddAudioClip(const std::string& audio_clip_name);
	void DeleteAudioClip(const std::string& audio_clip_name);
	const std::map<std::string, std::shared_ptr<AudioClip>>& GetAudioClipMap() const { return m_p_audio_clip_map; }
	void SetCurrentAudioClip(const std::string& audio_clip_name);
	void SetCurrentAudioClip(const std::string& audio_clip_name, const float& volume, const bool& is_loop = false);
	const std::shared_ptr<AudioClip>& GetCurrentAudioClip() const { SAFE_GET_POINTER(m_p_current_audio_clip); }

	void SetIsLoop(const bool& is_loop);
	const bool& GetIsLoop() { return m_is_loop; }

	void SetVolume(const float& volume);
	const float& GetVolume() { return m_volume; }

private:
	void SetSourceAttributes();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(AudioSource);

private:
	std::map<std::string, std::shared_ptr<AudioClip>> m_p_audio_clip_map;
	std::shared_ptr<AudioClip> m_p_current_audio_clip = nullptr;
	
	bool m_is_playing = false;
	bool m_is_loop = false;
	float m_volume = 1.0f;
};

