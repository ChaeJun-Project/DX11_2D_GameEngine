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
    void SetIsLoop(const bool& is_loop);
	const bool& GetIsLoop() { return m_is_loop; }

	void SetVolume(const float& volume);
	const float& GetVolume() { return m_volume; }

private:
    void SetSourceAttributes();

public:
	CLONE(AudioSource);

private:
    std::shared_ptr<AudioClip> m_p_audio_clip = nullptr;

	bool m_is_loop = false;
	float m_volume = 1.0f;
};

