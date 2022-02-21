#include "stdafx.h"
#include "GUI_Component.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>

GUI_Component::GUI_Component(const std::string& component_gui_name)
	:m_component_gui_name(component_gui_name)
{
}

const bool GUI_Component::BeginComponent(const std::string& component_name, const ComponentType& component_type, bool& is_active, const IconType& icon_type, const std::string& script_name)
{
	//Component Box�� Title �����
	const bool collapsed_header = ImGui::CollapsingHeader(component_name.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen); //â�� Ŵ�� ���ÿ� CollapsingHeader�� ������ Open
	if (collapsed_header)
	{
		//�ش� Component�� Script�� ���
		if (component_type == ComponentType::Script)
		{
			std::string guid_str = std::to_string(m_select_game_object->GetObjectID()) + script_name;
			ImGui::PushID(guid_str.c_str());
			{
				ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 8.0f); //���۳�Ʈ ����� ���� ����

				if (ICON_PROVIDER->CreateImageButton(IconType::Component_Settings, ImVec2(13.0f, 13.0f)))
					ImGui::OpenPopup(script_name.c_str(), ImGuiPopupFlags_MouseButtonRight);

				ShowScriptSettingPopup(script_name);

				//Active Check Box
				ImGui::Checkbox("", &is_active);
				ImGui::SameLine();
			}
			ImGui::PopID();
		}

		//�� �� ��� ���
		else
		{
			std::string guid_str = std::to_string(m_select_game_object->GetObjectID()) + component_name;
			ImGui::PushID(guid_str.c_str());
			if (component_type != ComponentType::Transform)
			{
				ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 8.0f); //���۳�Ʈ ����� ���� ����

				if (ICON_PROVIDER->CreateImageButton(IconType::Component_Settings, ImVec2(13.0f, 13.0f)))
					ImGui::OpenPopup(component_name.c_str(), ImGuiPopupFlags_MouseButtonRight);

				ShowComponentSettingPopup(component_name, component_type);

				//Active Check Box
				ImGui::Checkbox("", &is_active);
				ImGui::SameLine();
			}
			ImGui::PopID();
		}

		//Component Icon �׸���
		ICON_PROVIDER->CreateImage(icon_type, ImVec2(14.0f, 14.0f));
		ImGui::SameLine();
		//Component Name
		if (component_type == ComponentType::Script)
		{
			std::string script_title = component_name + "(" + script_name + ")";
			ImGui::Text(script_title.c_str()); //Script(Script �̸�)
		}
		else
			ImGui::Text(component_name.c_str()); //Component �̸�
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
	ImGui::Separator();  //���۳�Ʈ�� ������ ���� �� �׸���
}
