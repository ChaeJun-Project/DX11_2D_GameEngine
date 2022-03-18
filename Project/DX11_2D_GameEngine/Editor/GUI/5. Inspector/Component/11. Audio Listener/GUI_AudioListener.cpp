#include "stdafx.h"
#include "GUI_AudioListener.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/AudioListener.h>

GUI_AudioListener::GUI_AudioListener(const std::string& audio_listener_gui_name)
	:GUI_Component(audio_listener_gui_name)
{
}

void GUI_AudioListener::Render()
{
	auto audio_listener = m_p_selected_game_object->GetComponent<AudioListener>();
	if (audio_listener == nullptr)
		return;

	auto is_active = audio_listener->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::AudioListener, is_active, IconType::Component_Audio_Listener))
	{
		auto audio_listener = m_p_selected_game_object->GetComponent<AudioListener>();
		if (audio_listener == nullptr)
			return;

		audio_listener->SetIsActive(is_active);

		DrawComponentEnd();
	}
}
