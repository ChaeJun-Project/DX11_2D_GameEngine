#include "stdafx.h"
#include "GUI_AudioListener.h"

#include <DX11_2D_GameEngine_Lib/AudioListener.h>

GUI_AudioListener::GUI_AudioListener(const std::string& audio_listener_gui_name)
	:GUI_Component(audio_listener_gui_name)
{
}

void GUI_AudioListener::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::AudioListener, IconType::Component_Audio_Listener))
	{
		DrawComponentEnd();
	}
}
