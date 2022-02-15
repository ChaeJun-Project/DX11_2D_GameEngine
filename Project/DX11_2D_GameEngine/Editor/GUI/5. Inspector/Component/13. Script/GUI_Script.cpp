#include "stdafx.h"
#include "GUI_Script.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Script.h>

GUI_Script::GUI_Script(const std::string& script_gui_name)
	:GUI_Component(script_gui_name)
{
}

void GUI_Script::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Script, IconType::Component_Script, m_script_name))
	{
		auto script = m_select_game_object->GetScript(m_script_name);
		if (script == nullptr)
			return;

		DrawComponentEnd();
	}
}
