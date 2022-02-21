#include "stdafx.h"
#include "GUI_Component.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>

GUI_Component::GUI_Component(const std::string& component_gui_name)
	:m_component_gui_name(component_gui_name)
{
}

const bool GUI_Component::BeginComponent(const std::string& component_name, const ComponentType& component_type, bool& is_active, const IconType& icon_type, const std::string& script_name)
{
	//Component Box의 Title 만들기
	const bool collapsed_header = ImGui::CollapsingHeader(component_name.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen); //창을 킴과 동시에 CollapsingHeader의 내용을 Open
	if (collapsed_header)
	{
		//해당 Component가 Script인 경우
		if (component_type == ComponentType::Script)
		{
			std::string guid_str = std::to_string(m_select_game_object->GetObjectID()) + script_name;
			ImGui::PushID(guid_str.c_str());
			{
				ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 8.0f); //컴퍼넌트 헤더와 같은 라인

				if (ICON_PROVIDER->CreateImageButton(IconType::Component_Settings, ImVec2(13.0f, 13.0f)))
					ImGui::OpenPopup(script_name.c_str(), ImGuiPopupFlags_MouseButtonRight);

				ShowScriptSettingPopup(script_name);

				//Active Check Box
				ImGui::Checkbox("", &is_active);
				ImGui::SameLine();
			}
			ImGui::PopID();
		}

		//그 외 모든 경우
		else
		{
			std::string guid_str = std::to_string(m_select_game_object->GetObjectID()) + component_name;
			ImGui::PushID(guid_str.c_str());
			if (component_type != ComponentType::Transform)
			{
				ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 8.0f); //컴퍼넌트 헤더와 같은 라인

				if (ICON_PROVIDER->CreateImageButton(IconType::Component_Settings, ImVec2(13.0f, 13.0f)))
					ImGui::OpenPopup(component_name.c_str(), ImGuiPopupFlags_MouseButtonRight);

				ShowComponentSettingPopup(component_name, component_type);

				//Active Check Box
				ImGui::Checkbox("", &is_active);
				ImGui::SameLine();
			}
			ImGui::PopID();
		}

		//Component Icon 그리기
		ICON_PROVIDER->CreateImage(icon_type, ImVec2(14.0f, 14.0f));
		ImGui::SameLine();
		//Component Name
		if (component_type == ComponentType::Script)
		{
			std::string script_title = component_name + "(" + script_name + ")";
			ImGui::Text(script_title.c_str()); //Script(Script 이름)
		}
		else
			ImGui::Text(component_name.c_str()); //Component 이름
	}

	return collapsed_header;
}

void GUI_Component::ShowComponentSettingPopup(const std::string& component_popup_id, const ComponentType& component_type)
{
	if (ImGui::BeginPopup(component_popup_id.c_str()))
	{
		//Remove Component
		if (ImGui::MenuItem("Remove Component"))
		{
			m_select_game_object->RemoveComponent(component_type);
		}
		ImGui::EndPopup();
	}
}

void GUI_Component::ShowScriptSettingPopup(const std::string& script_name)
{
	if (ImGui::BeginPopup(script_name.c_str()))
	{
		//Remove Script
		if (ImGui::MenuItem("Remove Script"))
		{
			m_select_game_object->RemoveScript(script_name);
		}
		ImGui::EndPopup();
	}
}

void GUI_Component::DrawComponentEnd()
{
	ImGui::Separator();  //컴퍼넌트를 구분할 구분 선 그리기
}
