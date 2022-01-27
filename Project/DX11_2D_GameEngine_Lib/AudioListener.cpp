#include "stdafx.h"
#include "AudioListener.h"

#include "GameObject.h"
#include "Transform.h"

AudioListener::AudioListener()
	:IComponent(ComponentType::AudioListener)
{

}

AudioListener::AudioListener(const AudioListener& origin)
	: IComponent(origin.GetComponentType())
{

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