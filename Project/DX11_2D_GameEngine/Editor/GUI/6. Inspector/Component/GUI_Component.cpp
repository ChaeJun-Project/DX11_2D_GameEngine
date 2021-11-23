#include "stdafx.h"
#include "GUI_Component.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>


GUI_Component::GUI_Component(const std::string& component_gui_name)
	:m_component_gui_name(component_gui_name)
{
}

const bool GUI_Component::BeginComponent(const std::string& component_name, const ComponentType& component_type, const IconType& icon_type)
{
	//Component Box의 Title 만들기
	const bool collapsed_header = ImGui::CollapsingHeader(component_name.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen); //창을 킴과 동시에 CollapsingHeader의 내용을 Open
	if (collapsed_header)
	{
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 8.0f); //컴퍼넌트 헤더와 같은 라인

		auto icon_provider = IconProvider::GetInstance();

		//Component Settings 그리기
		if (icon_provider->CreateImageButton(component_name.c_str(), IconType::Component_Settings, ImVec2(13.0f, 13.0f)))
			ImGui::OpenPopup(component_name.c_str());

		ShowComponentSettingPopup(component_name.c_str(), component_type);

		//Component Icon 그리기
		icon_provider->CreateImage(icon_type, ImVec2(14.0f, 14.0f));

		ImGui::SameLine(); //컴퍼넌트 헤더와 같은 라인

		//Component 이름
		ImGui::Text(component_name.c_str());
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
			if (component_type != ComponentType::Transform)
				m_select_game_object->RemoveComponent(component_type);
		}

		ImGui::EndPopup();
	}
}

void GUI_Component::DrawComponentEnd()
{
	ImGui::Separator();  //컴퍼넌트를 구분할 구분 선 그리기
}
