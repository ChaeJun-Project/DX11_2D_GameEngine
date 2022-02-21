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
	auto script = m_select_game_object->GetScript(m_script_name);
	if (script == nullptr)
		return;

	auto is_active = script->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::Script, is_active, IconType::Component_Script, m_script_name))
	{
		auto script = m_select_game_object->GetScript(m_script_name);
		if (script == nullptr)
			return;

		script->SetIsActive(is_active);

		const auto& script_param_vector = script->GetScriptParamVector();
		
		for (UINT i = 0; i < static_cast<UINT>(script_param_vector.size()); ++i)
		{
			switch (script_param_vector[i].m_param_type)
			{
			case ScriptParamType::Int:
				break;
			case ScriptParamType::Float:
				break;
			case ScriptParamType::Vector2:
				break;
			case ScriptParamType::Vector3:
				break;
			case ScriptParamType::Vector4:
				break;
			case ScriptParamType::Texture:
				break;
			case ScriptParamType::Prefab:
				break;
			}
		}


		DrawComponentEnd();
	}
}