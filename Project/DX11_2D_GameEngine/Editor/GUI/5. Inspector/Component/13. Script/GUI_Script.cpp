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
	if (BeginComponent("Script", ComponentType::Script, IconType::Component_Script))
	{


		//ImGui::Text("Script");
		//ImGui::SameLine(80.0f);
		//ImGui::InputText("##Script", , ImGuiInputTextFlags_ReadOnly);
		DrawComponentEnd();
	}
}
