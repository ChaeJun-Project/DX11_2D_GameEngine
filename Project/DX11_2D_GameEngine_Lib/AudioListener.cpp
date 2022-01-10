#include "stdafx.h"
#include "AudioListener.h"

#include "AudioManager.h"

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
	AudioManager::GetInstance()->SetListenerTransform(m_p_owner_game_object->GetComponent<Transform>());
}

void AudioListener::FinalUpdate()
{
	AudioManager::GetInstance()->SetListenerTransform(m_p_owner_game_object->GetComponent<Transform>());
}
