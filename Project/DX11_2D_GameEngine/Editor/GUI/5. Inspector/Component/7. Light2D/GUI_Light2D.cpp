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
	if (BeginComponent(m_component_gui_name, ComponentType::Light2D, IconType::Component_Light2D))
	{
		auto light2D = m_select_game_object->GetComponent<Light2D>();
		auto light2D_info = light2D->GetLight2DInfo();

		//Light Type
		ShowComboLightType(light2D, light2D_info.light_type);

		//Light Color
		//Color
		ShowColorPicker("Color", (float*)(&(light2D_info.ligth_color.color)), m_color_edit_flag);
		//Specular
		ShowColorPicker("Specular", (float*)(&(light2D_info.ligth_color.specular)), m_color_edit_flag);
		//Ambient
		ShowColorPicker("Ambient", (float*)(&(light2D_info.ligth_color.ambient)), m_color_edit_flag);
		
		//Light Direction
		ShowFloat3("Direction", light2D_info.light_direction, 50.0f, 100.0f);

		//Light Ragne
		ShowFloat("Light2D", "Range", light2D_info.light_range, 70.0f, 100.0f);

		//Light Angle
		ShowFloat("Light2D", "Angle", light2D_info.light_angle, 70.0f, 100.0f);

		//Set Data
		//Light Color
		if (SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop)
		{
			light2D->SetLightColor(light2D_info.ligth_color.color);
			light2D->SetLightSpecular(light2D_info.ligth_color.specular);
			light2D->SetLightAmbient(light2D_info.ligth_color.ambient);

			light2D->SetLightDir(light2D_info.light_direction);
			light2D->SetLightRange(light2D_info.light_range);
			light2D->SetLightAngle(light2D_info.light_angle);
		}

		DrawComponentEnd();
	}
}

void GUI_Light2D::ShowComboLightType(Light2D* p_light2D, LightType& light_type)
{
	ImGui::BeginGroup();
	ImGui::Text("Type");
	ImGui::SameLine(90.0f);

	//Show Combo
	ImGui::PushItemWidth(120.0f);
	{
		int index = 0;

		std::string current_type_name;

		switch (light_type)
		{
		case LightType::Directional:
			current_type_name = "Directional";
			break;
		case LightType::Point:
			current_type_name = "Point";
			break;
		case LightType::Spot:
			current_type_name = "Spot";
			break;

		}

		m_p_light_type_list->AddItem("Directional");
		m_p_light_type_list->AddItem("Point");
		m_p_light_type_list->AddItem("Spot");

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
				const bool is_selected = (*(m_p_light_type_list->GetCurrentListID()) == i);
				if (ImGui::Selectable(light_type_list_vector[i].c_str(), is_selected))
				{
					if (SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop)
						p_light2D->SetLightType(static_cast<LightType>(i));
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

void GUI_Light2D::ShowColorPicker(const std::string& label_name, float* data, const ImGuiColorEditFlags& flags)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	std::string label_tag = "##Light " + label_name;
	ImGui::ColorEdit3(label_tag.c_str(), data, m_color_edit_flag);
}
