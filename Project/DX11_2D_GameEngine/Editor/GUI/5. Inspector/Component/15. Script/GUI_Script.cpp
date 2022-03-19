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
	auto script = m_p_selected_game_object->GetScript(m_script_name);
	if (script == nullptr)
		return;

	auto is_active = script->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::Script, is_active, IconType::Component_Script, m_script_name))
	{
		auto script = m_p_selected_game_object->GetScript(m_script_name);
		if (script == nullptr)
			return;

		script->SetIsActive(is_active);

		const auto& script_param_vector = script->GetScriptParamVector();
		
		for (UINT i = 0; i < static_cast<UINT>(script_param_vector.size()); ++i)
		{
			switch (script_param_vector[i].m_param_type)
			{
			case ScriptParamType::Int:
				DataInputInt(script_param_vector[i].m_param_name, reinterpret_cast<int*>(std::get<void*>(script_param_vector[i].m_p_param_data)), 60.0f, script_param_vector[i].m_indent);
				break;
			case ScriptParamType::Float:
				DataInputFloat(script_param_vector[i].m_param_name, reinterpret_cast<float*>(std::get<void*>(script_param_vector[i].m_p_param_data)), 60.0f, script_param_vector[i].m_indent);
				break;
			case ScriptParamType::Vector2:
				DataInputVector2(script_param_vector[i].m_param_name, reinterpret_cast<Vector2*>(std::get<void*>(script_param_vector[i].m_p_param_data)), 60.0f, script_param_vector[i].m_indent);
				break;
			case ScriptParamType::Vector3:
				DataInputVector3(script_param_vector[i].m_param_name, reinterpret_cast<Vector3*>(std::get<void*>(script_param_vector[i].m_p_param_data)), 60.0f, script_param_vector[i].m_indent);
				break;
			case ScriptParamType::Vector4:
				DataInputVector4(script_param_vector[i].m_param_name, reinterpret_cast<Vector4*>(std::get<void*>(script_param_vector[i].m_p_param_data)), 60.0f, script_param_vector[i].m_indent);
			    break;
			case ScriptParamType::String:
				DataInputString(script_param_vector[i].m_param_name, reinterpret_cast<std::string*>(std::get<void*>(script_param_vector[i].m_p_param_data)), 100.0f, script_param_vector[i].m_indent);
				break;
			case ScriptParamType::Texture:
				DataInputTexture(script_param_vector[i].m_param_name, reinterpret_cast<Texture**>(std::get<void*>(script_param_vector[i].m_p_param_data)), 100.0f, script_param_vector[i].m_indent);
				break;
			case ScriptParamType::Prefab:
				DataInputPrefab(script_param_vector[i].m_param_name, reinterpret_cast<Prefab**>(std::get<void*>(script_param_vector[i].m_p_param_data)), 100.0f, script_param_vector[i].m_indent);
				break;
			}
		}


		DrawComponentEnd();
	}
}