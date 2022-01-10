#include "stdafx.h"
#include "AudioClip.h"

AudioClip::AudioClip(const std::string resource_name)
	:IResource(ResourceType::AudioClip, resource_name)
{
	m_loop_mode = FMOD_LOOP_OFF;
}

AudioClip::~AudioClip()
{
	if (m_p_sound)
		m_p_sound->release();
}

void AudioClip::Play()
{
	AUDIO_SYSTEM->playSound(m_p_sound, nullptr, true, &m_p_channel);
}

void AudioClip::Pause()
{
    bool is_pause = false;
	m_p_channel->getPaused(&is_pause);

	//현재 중지된 상태라면
	if (is_pause)
		return;

	is_pause = true;
	m_p_channel->setPaused(is_pause);
}

void AudioClip::Stop()
{
	m_p_channel->stop();

	m_p_channel = nullptr;
}

void AudioClip::SetLoop(const bool& is_loop)
{
	if (is_loop)
		m_loop_mode = FMOD_LOOP_NORMAL;

	else
		m_loop_mode = FMOD_LOOP_OFF;


	m_p_sound->setMode(m_loop_mode);
}

void AudioClip::SetVolume(const float& volume)
{
	m_p_channel->setVolume(volume);
	m_p_channel->set3DMinMaxDistance(m_min_distance, m_max_distance);
}

void AudioClip::CreateAudio(const std::string& audio_clip_path)
{
	auto result = AUDIO_SYSTEM->createSound(audio_clip_path.c_str(), FMOD_3D, 0, &m_p_sound);
	assert(result == FMOD_OK);

	m_p_sound->set3DMinMaxDistance(m_min_distance, m_max_distance);
}

void AudioClip::LoadFromFile(const std::string& audio_clip_path)
{
	CreateAudio(audio_clip_path);
}