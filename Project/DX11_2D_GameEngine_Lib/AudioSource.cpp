#include "stdafx.h"
#include "AudioSource.h"

#include "SceneManager.h"

#include "AudioClip.h"

#include "GameObject.h"
#include "Transform.h"

AudioSource::AudioSource()
	:IComponent(ComponentType::AudioSource)
{

}

AudioSource::AudioSource(const AudioSource& origin)
	: IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;

	m_p_audio_clip = origin.m_p_audio_clip;
	
	m_is_loop = origin.m_is_loop;
	m_volume = origin.m_volume;
}

AudioSource::~AudioSource()
{
	m_p_audio_clip.reset();
}

void AudioSource::Start()
{
	if (m_p_audio_clip == nullptr)
		return;

	Play();

	SetSourceAttributes();
}

void AudioSource::FinalUpdate()
{
	if (m_p_audio_clip == nullptr || m_p_audio_clip->m_p_channel == nullptr)
		return;

	auto scene_manager = SCENE_MANAGER;

	//Editor Mode에 정지 상태일 경우
	if ((scene_manager->GetClientState() == 2 && scene_manager->GetEditorState() == EditorState::EditorState_Stop))
	{
		Stop();
		return;
	}
		

	if ((scene_manager->GetEditorState() & EditorState::EditorState_Play))
	{
		if((scene_manager->GetEditorState() & EditorState::EditorState_Pause))
			Pause();

		else
			Resume();
	}
		
	SetSourceAttributes();
}

void AudioSource::Play()
{
	if (m_p_audio_clip == nullptr)
		return;

	m_p_audio_clip->SetLoop(m_is_loop);
	m_p_audio_clip->Play();
	m_p_audio_clip->SetVolume(m_volume);
	m_is_playing = true;
}

void AudioSource::Pause()
{
	if (m_p_audio_clip == nullptr || m_p_audio_clip->m_p_channel == nullptr || !m_is_playing)
		return;

	m_p_audio_clip->Pause();
	m_is_playing = false;
}

void AudioSource::Resume()
{
	if (m_p_audio_clip == nullptr || m_p_audio_clip->m_p_channel == nullptr || m_is_playing)
		return;

	m_p_audio_clip->Resume();
	m_is_playing = true;
}

void AudioSource::Stop()
{
	if (m_p_audio_clip == nullptr || m_p_audio_clip->m_p_channel == nullptr || !m_is_playing)
		return;

	m_p_audio_clip->Stop();
	m_is_playing = false;
}

void AudioSource::SetAudioClip(const std::shared_ptr<AudioClip>& p_audio_clip)
{
	if (p_audio_clip == nullptr)
		return;

	m_p_audio_clip = p_audio_clip;
}

void AudioSource::SetIsLoop(const bool& is_loop)
{
	if (m_p_audio_clip == nullptr)
		return;

	m_is_loop = is_loop;
	m_p_audio_clip->SetLoop(m_is_loop);
}

void AudioSource::SetVolume(const float& volume)
{
	if (m_p_audio_clip == nullptr)
		return;

	m_volume = volume;
	m_p_audio_clip->SetVolume(m_volume);
}

void AudioSource::SetSourceAttributes()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>(); //Main Camera의 현재 위치
	auto source_position = transform->GetTranslation();
	auto velocity = Vector3::Zero;

	auto result = m_p_audio_clip->m_p_channel->set3DAttributes
	(
		reinterpret_cast<FMOD_VECTOR*>(&source_position),
		reinterpret_cast<FMOD_VECTOR*>(&velocity)
	);

}

void AudioSource::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Audio
	fprintf(p_file, "[Audio]\n");
	RESOURCE_MANAGER->SaveResource<AudioClip>(m_p_audio_clip, p_file);
	
	//Loop
	fprintf(p_file, "[Loop]\n");
	fprintf(p_file, "%d\n", m_is_loop);

	//Volume
	fprintf(p_file, "[Volume]\n");
	fprintf(p_file, "%f\n", m_volume);
}

void AudioSource::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Audio
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<AudioClip>(m_p_audio_clip, p_file);

	//Loop
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &m_is_loop);

	//Volume
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_volume);
}
