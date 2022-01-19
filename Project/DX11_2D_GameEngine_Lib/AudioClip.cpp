#include "stdafx.h"
#include "AudioClip.h"

AudioClip::AudioClip(const std::string& audio_clip_resource_name)
	:IResource(ResourceType::AudioClip, audio_clip_resource_name)
{
	m_loop_mode = FMOD_LOOP_OFF;
}

AudioClip::AudioClip(const AudioClip& origin)
	: IResource(origin.GetResourceType(), origin.GetResourceName())
{
	if (origin.m_p_sound != nullptr)
		m_p_sound = origin.m_p_sound;

	m_min_distance = origin.m_min_distance;
	m_max_distance = origin.m_max_distance;

	m_loop_mode = origin.m_loop_mode;
	m_audio_mode = origin.m_audio_mode;
}

AudioClip::~AudioClip()
{
	if (m_p_sound != nullptr)
		m_p_sound->release(); //Audio Manager ���� ���� Release �Ǿ�� �� => ResourceManager�� �����͸� �����ϸ� �ذ��
}

void AudioClip::Play()
{
	AUDIO_SYSTEM->playSound(m_p_sound, nullptr, false, &m_p_channel);
}

void AudioClip::Pause()
{
	bool is_pause = false;
	m_p_channel->getPaused(&is_pause);

	//���� ������ ���¶��
	if (is_pause)
		return;

	is_pause = true;
	m_p_channel->setPaused(is_pause);
}

void AudioClip::Resume()
{
	bool is_pause = false;
	m_p_channel->getPaused(&is_pause);

	//���� ������ ���°� �ƴ϶��
	if (!is_pause)
		return;

	is_pause = false;
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

void AudioClip::SetAudioType(const AudioModeType& audio_mode_type)
{
	switch (audio_mode_type)
	{
	case AudioModeType::Audio_2D:
		m_loop_mode = FMOD_2D;
		break;
	case AudioModeType::Audio_3D:
		m_loop_mode = FMOD_3D;
		break;
	}
}

void AudioClip::CreateAudio(const std::string& audio_clip_path)
{
	auto result = AUDIO_SYSTEM->createSound(audio_clip_path.c_str(), m_audio_mode, 0, &m_p_sound);
	assert(result == FMOD_OK);

	m_p_sound->set3DMinMaxDistance(m_min_distance, m_max_distance);
}

bool AudioClip::LoadFromFile(const std::string& audio_clip_path)
{
	CreateAudio(audio_clip_path);

	return true;
}