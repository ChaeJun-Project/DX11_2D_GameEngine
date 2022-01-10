#include "stdafx.h"
#include "AudioSource.h"

#include "AudioClip.h"

#include "Transform.h"

AudioSource::AudioSource()
	:IComponent(ComponentType::AudioSource)
{

}

AudioSource::AudioSource(const AudioSource& origin)
	: IComponent(origin.GetComponentType())
{

}

AudioSource::~AudioSource()
{
	m_p_audio_clip.reset();
}

void AudioSource::Start()
{
	if (m_p_audio_clip == nullptr)
		return;

	SetSourceAttributes();
}

void AudioSource::FinalUpdate()
{
	if (m_p_audio_clip == nullptr)
		return;

	SetSourceAttributes();
}

void AudioSource::SetIsLoop(const bool& is_loop)
{
	if (m_p_audio_clip == nullptr)
		return;

	m_p_audio_clip->SetLoop(is_loop);
}

void AudioSource::SetVolume(const float& volume)
{
	if (m_p_audio_clip == nullptr)
		return;

	m_p_audio_clip->SetVolume(volume);
}

void AudioSource::SetSourceAttributes()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>(); //Main Camera의 현재 위치
	auto source_position = transform->GetTranslation();
	auto velocity = Vector3::Zero;

	m_p_audio_clip->m_p_channel->set3DAttributes
	(
		reinterpret_cast<FMOD_VECTOR*>(&source_position),
		reinterpret_cast<FMOD_VECTOR*>(&velocity)
	);
}
