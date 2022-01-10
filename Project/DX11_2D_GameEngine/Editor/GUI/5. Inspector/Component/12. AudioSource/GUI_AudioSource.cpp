#include "stdafx.h"
#include "GUI_AudioSource.h"

#include <DX11_2D_GameEngine_Lib/AudioSource.h>

GUI_AudioSource::GUI_AudioSource(const std::string& audio_source_gui_name)
	:GUI_Component(audio_source_gui_name)
{
}

void GUI_AudioSource::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::AudioSource, IconType::Component_Audio_Source))
	{
		auto audio_source = m_select_game_object->GetComponent<AudioSource>();

		DrawComponentEnd();
	}
}
