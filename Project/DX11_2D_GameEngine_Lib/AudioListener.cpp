#include "stdafx.h"
#include "AudioListener.h"

#include "GameObject.h"
#include "Transform.h"

AudioListener::AudioListener()
	:IComponent(ComponentType::AudioListener)
{

}

AudioListener::AudioListener(const AudioListener& origin)
	: IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;
}

void AudioListener::Start()
{
	AUDIO_MANAGER->SetListenerTransform(m_p_owner_game_object->GetComponent<Transform>());
}

void AudioListener::FinalUpdate()
{
	AUDIO_MANAGER->SetListenerTransform(m_p_owner_game_object->GetComponent<Transform>());
}

void AudioListener::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent
}

void AudioListener::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent
}