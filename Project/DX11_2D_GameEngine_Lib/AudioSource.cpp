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

	//Audio Clip Map
	for (const auto& audio_clip : origin.m_p_audio_clip_map)
	{
		auto audio_clip_iter = m_p_audio_clip_map.insert(std::make_pair(audio_clip.first, audio_clip.second));
		auto result = audio_clip_iter.second;
		assert(result);
	}

	//Current Audio Clip
	if (origin.m_p_current_audio_clip != nullptr)
		m_p_current_audio_clip = m_p_audio_clip_map[origin.m_p_current_audio_clip->GetResourceName()];

	m_is_loop = origin.m_is_loop;
	m_volume = origin.m_volume;
}

AudioSource::~AudioSource()
{
	//Audio Clip Map
	for (auto& p_audio_clip : m_p_audio_clip_map)
	{
		p_audio_clip.second->Stop();
		p_audio_clip.second.reset();
	}
	m_p_audio_clip_map.clear();

	//Current Audio Clip
	m_p_current_audio_clip.reset();
}

void AudioSource::Start()
{
	Play();
}

void AudioSource::FinalUpdate()
{
	if (m_p_current_audio_clip == nullptr || m_p_current_audio_clip->m_p_channel == nullptr)
		return;

	//Editor Mode에 정지 상태일 경우
	if ((SCENE_MANAGER->GetClientState() == 2 && SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Stop))
	{
		Stop();
		return;
	}

	if ((SCENE_MANAGER->GetEditorState() & EditorState::EditorState_Play))
	{
		if ((SCENE_MANAGER->GetEditorState() & EditorState::EditorState_Pause))
			Pause();

		else
			Resume();
	}

	SetSourceAttributes();
}

void AudioSource::Play()
{
	if (m_p_current_audio_clip == nullptr)
		return;

	m_p_current_audio_clip->SetLoop(m_is_loop);
	m_p_current_audio_clip->Play();
	m_p_current_audio_clip->SetVolume(m_volume);
	m_is_playing = true;
}

void AudioSource::Pause()
{
	if (m_p_current_audio_clip == nullptr || m_p_current_audio_clip->m_p_channel == nullptr || !m_is_playing)
		return;

	m_p_current_audio_clip->Pause();
	m_is_playing = false;
}

void AudioSource::Resume()
{
	if (m_p_current_audio_clip == nullptr || m_p_current_audio_clip->m_p_channel == nullptr || m_is_playing)
		return;

	m_p_current_audio_clip->Resume();
	m_is_playing = true;
}

void AudioSource::Stop()
{
	if (m_p_current_audio_clip == nullptr || m_p_current_audio_clip->m_p_channel == nullptr || !m_is_playing)
		return;

	m_p_current_audio_clip->Stop();
	m_is_playing = false;
}

void AudioSource::AddAudioClip(const std::string& audio_clip_name)
{
	auto p_audio_clip = RESOURCE_MANAGER->GetResource<AudioClip>(audio_clip_name);

	if (p_audio_clip == nullptr)
		return;

	m_p_audio_clip_map.insert(std::make_pair(audio_clip_name, p_audio_clip));
}

void AudioSource::DeleteAudioClip(const std::string& audio_clip_name)
{
	auto audio_clip_iter = m_p_audio_clip_map.find(audio_clip_name);
	if (audio_clip_iter == m_p_audio_clip_map.end())
		return;

	m_p_audio_clip_map.erase(audio_clip_name);
}

void AudioSource::SetCurrentAudioClip(const std::string& audio_clip_name)
{
	if (m_p_current_audio_clip != nullptr)
	{
		if (m_p_current_audio_clip->GetResourceName()._Equal(audio_clip_name))
			return;

		else
			m_p_current_audio_clip->Stop();
	}

	auto audio_clip_iter = m_p_audio_clip_map.find(audio_clip_name);
	if (audio_clip_iter == m_p_audio_clip_map.end())
		return;

	m_p_current_audio_clip = audio_clip_iter->second;

	Play();
}

void AudioSource::SetCurrentAudioClip(const std::string& audio_clip_name, const float& volume, const bool& is_loop)
{
	if (m_p_current_audio_clip != nullptr)
	{
		if (m_p_current_audio_clip->GetResourceName()._Equal(audio_clip_name))
			return;

		else
			m_p_current_audio_clip->Stop();
	}

	auto audio_clip_iter = m_p_audio_clip_map.find(audio_clip_name);
	if (audio_clip_iter == m_p_audio_clip_map.end())
		return;

	m_p_current_audio_clip = audio_clip_iter->second;

	SetVolume(volume);
	SetIsLoop(is_loop);

	Play();
}

void AudioSource::SetIsLoop(const bool& is_loop)
{
	if (m_p_current_audio_clip == nullptr)
		return;

	m_is_loop = is_loop;
	m_p_current_audio_clip->SetLoop(m_is_loop);
}

void AudioSource::SetVolume(const float& volume)
{
	if (m_p_current_audio_clip == nullptr)
		return;

	m_volume = volume;
	m_p_current_audio_clip->SetVolume(m_volume);
}

void AudioSource::SetSourceAttributes()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>(); //Main Camera의 현재 위치
	auto source_position = transform->GetTranslation();
	auto velocity = Vector3::Zero;

	auto result = m_p_current_audio_clip->m_p_channel->set3DAttributes
	(
		reinterpret_cast<FMOD_VECTOR*>(&source_position),
		reinterpret_cast<FMOD_VECTOR*>(&velocity)
	);
}

void AudioSource::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Audio Clip Map
	fprintf(p_file, "[Audio Map]\n");

	//Count
	fprintf(p_file, "[Count]\n");
	fprintf(p_file, "%d\n", m_p_audio_clip_map.size());

	//Data
	fprintf(p_file, "[Data]\n");
	for (const auto& p_audio_clip : m_p_audio_clip_map)
	{
		//Key
		fprintf(p_file, "[Key]\n");
		fprintf(p_file, "%s\n", p_audio_clip.first.c_str());
		//Save Sprite Animation Resource
		RESOURCE_MANAGER->SaveResource<AudioClip>(p_audio_clip.second, p_file);
	}

	//Current Audio Clip
	fprintf(p_file, "[Current Audio]\n");
	if (m_p_current_audio_clip != nullptr)
		fprintf(p_file, "%s\n", m_p_current_audio_clip->GetResourceName().c_str());
	else
		fprintf(p_file, "%s\n", "None");

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

	//Audio Clip Map
	FILE_MANAGER->FScanf(char_buffer, p_file);

	//Count
	FILE_MANAGER->FScanf(char_buffer, p_file);
	UINT audio_clip_count = 0;
	fscanf_s(p_file, "%d\n", &audio_clip_count);

	//Data
	FILE_MANAGER->FScanf(char_buffer, p_file);
	for (UINT i = 0; i < audio_clip_count; ++i)
	{
		//Key
		FILE_MANAGER->FScanf(char_buffer, p_file);
		FILE_MANAGER->FScanf(char_buffer, p_file);
		auto audio_clip_key = std::string(char_buffer);

		//Load Audio Clip Resource
		std::shared_ptr<AudioClip> p_audio_clip = nullptr;
		RESOURCE_MANAGER->LoadResource<AudioClip>(p_audio_clip, p_file);
		m_p_audio_clip_map.insert(std::make_pair(audio_clip_key, p_audio_clip));
	}

	//Current Audio Clip
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);
	auto current_audio_clip_key = std::string(char_buffer);
	if (!current_audio_clip_key._Equal("None"))
		m_p_current_audio_clip = m_p_audio_clip_map[current_audio_clip_key];

	//Loop
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &m_is_loop);

	//Volume
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_volume);
}
