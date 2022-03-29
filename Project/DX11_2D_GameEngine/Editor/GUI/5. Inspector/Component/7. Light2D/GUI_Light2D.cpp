#include "stdafx.h"
#include "GUI_Light2D.h"

#include "Helper/IconProvider.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Light2D.h>

GUI_Light2D::GUI_Light2D(const std::string& light2D_gui_name)
	:GUI_Component(light2D_gui_name)
{
	m_p_light_type_list = new GUI_ItemList();
}

GUI_Light2D::~GUI_Light2D()
{
	SAFE_DELETE(m_p_light_type_list);
}

void GUI_Light2D::Render()
{
	auto light2D = m_p_selected_game_object->GetComponent<Light2D>();
	if (light2D == nullptr)
		return;

	auto is_active = light2D->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::Light2D, is_active, IconType::Component_Light2D))
	{
		auto light2D = m_p_selected_game_object->GetComponent<Light2D>();
		if (light2D == nullptr)
			return;

		light2D->SetIsActive(is_active);

		auto light2D_info = light2D->GetLight2DInfo();

		//Light2D Type
		ShowComboLightType(light2D, light2D_info.type);

		//Light2D
		//Color
		ShowColorPicker3("Color", (float*)(&(light2D_info.color)), m_color_edit_flag);
		
		//Light Ragne
		ShowFloat("Light2D", "Range", light2D_info.range, 70.0f, 100.0f);

		//Set Data
		//Light Color
		if (CAN_EDIT)
		{
			light2D->SetLightColor(light2D_info.color);
			light2D->SetLightRange(light2D_info.range);
		}

		DrawComponentEnd();
	}
}

void GUI_Light2D::ShowComboLightType(Light2D* p_light2D, Light2DType& light2D_type)
{
	ImGui::BeginGroup();
	ImGui::Text("Type");
	ImGui::SameLine(90.0f);

	//Show Combo
	ImGui::PushItemWidth(120.0f);
	{
		int index = 0;

		std::string current_type_name;

		switch (light2D_type)
		{
		case Light2DType::Ambient:
			current_type_name = "Ambient";
			break;
		case Light2DType::Point:
			current_type_name = "Point";
			break;
		}

		m_p_light_type_list->AddItem("Ambient");
		m_p_light_type_list->AddItem("Point");

		const auto& light_type_list_vector = m_p_light_type_list->GetItemList();

		for (auto& light_type : light_type_list_vector)
		{
			if (light_type == current_type_name)
				m_p_light_type_list->SetCurrentListID(index);

			else
				++index;
		}

		if (ImGui::BeginCombo("##Light Type", current_type_name.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(light_type_list_vector.size()); ++i)
			{
				const bool is_selected = (m_p_light_type_list->GetCurrentListID() == i);
				if (ImGui::Selectable(light_type_list_vector[i].c_str(), is_selected))
				{
					if (SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Stop)
						p_light2D->SetLightType(static_cast<Light2DType>(i));
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		m_p_light_type_list->ClearItemList();
	}
	ImGui::PopItemWidth();
	ImGui::EndGroup();
}
