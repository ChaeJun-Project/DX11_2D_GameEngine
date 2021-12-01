#include "stdafx.h"
#include "GUI_Camera.h"

#include "GUI/GUI_ItemList.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>

GUI_Camera::GUI_Camera(const std::string& camera_gui_name)
	:GUI_Component(camera_gui_name)
{
	m_p_item_list = new GUI_ItemList;
}

GUI_Camera::~GUI_Camera()
{
	SAFE_DELETE(m_p_item_list);
}

void GUI_Camera::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Camera, IconType::Component_Camera))
	{
	    auto camera = m_select_game_object->GetComponent<Camera>();
		auto projection_type = camera->GetProjectionType();
		
		auto size = camera->GetSize();
		auto fov = Math::ToDegree(camera->GetFov());
		auto near_z = camera->GetNearZ();
		auto far_z = camera->GetFarZ();
		
		//Projection
		ImGui::Text("Projection");
		ImGui::SameLine(100.0f); //같은 라인에서 80만큼 떨어져서 시작

		std::string projection_type_str;

		switch (projection_type)
		{
		case ProjectionType::Orthographic:
			projection_type_str = "Orthographic";
			break;
		case ProjectionType::Perspective:
			projection_type_str = "Perspective";
			break;
		}

		m_p_item_list->AddItem("Orthographic");
		m_p_item_list->AddItem("Perspective");

		const auto& item_list_vector = m_p_item_list->GetItemList();

		if (ImGui::BeginCombo("##ProjectionType", projection_type_str.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
			{
				const bool is_selected = (*(m_p_item_list->GetCurrentListID()) == i);
				if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
				{
					SetProjectionType(camera, item_list_vector[i]);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		m_p_item_list->ClearItemList();

		//fov, near_z, far_z
		switch (camera->GetProjectionType())
		{
		case ProjectionType::Orthographic:
			ShowFloat2("Size", size, 100.0f);
			break;
		case ProjectionType::Perspective:
			projection_type_str = "Perspective";
			break;
		}

		ShowFloat("Camera", "Fov", fov, 100.0f);
		ShowFloat("Camera", "Near", near_z, 100.0f);
		ShowFloat("Camera", "Far", far_z, 100.0f);

		camera->SetSize(size);
		camera->SetFov(Math::ToRadian(fov));
		camera->SetNearZ(near_z);
		camera->SetFarZ(far_z);

		DrawComponentEnd();
	}
}

void GUI_Camera::SetProjectionType(Camera* camera, const std::string& projection_type_str)
{
	if(projection_type_str == "Orthographic")
		camera->SetProjectionType(ProjectionType::Orthographic);

	else if(projection_type_str == "Perspective")
		camera->SetProjectionType(ProjectionType::Perspective);
}
