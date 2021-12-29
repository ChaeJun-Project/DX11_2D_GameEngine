#include "stdafx.h"
#include "GUI_Camera.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>

GUI_Camera::GUI_Camera(const std::string& camera_gui_name)
	:GUI_Component(camera_gui_name)
{
	m_p_projection_list = new GUI_ItemList;
	m_p_culling_layer_list = new GUI_ItemList;
}

GUI_Camera::~GUI_Camera()
{
	SAFE_DELETE(m_p_projection_list);
	SAFE_DELETE(m_p_culling_layer_list);
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
		auto camera_index = camera->GetCameraIndex();
		auto culling_layer = camera->GetCullingLayer();

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

		m_p_projection_list->AddItem("Orthographic");
		m_p_projection_list->AddItem("Perspective");

		const auto& projection_list_vector = m_p_projection_list->GetItemList();

		if (ImGui::BeginCombo("##ProjectionType", projection_type_str.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(projection_list_vector.size()); ++i)
			{
				const bool is_selected = (*(m_p_projection_list->GetCurrentListID()) == i);
				if (ImGui::Selectable(projection_list_vector[i].c_str(), is_selected))
				{
					SetProjectionType(camera, projection_list_vector[i]);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		m_p_projection_list->ClearItemList();


		switch (camera->GetProjectionType())
		{
		case ProjectionType::Orthographic:
			ShowFloat2("Size", size, 100.0f, 70.0f);
			break;
		case ProjectionType::Perspective:
			projection_type_str = "Perspective";
			break;
		}

		//fov, near_z, far_z, Camera_Index
		ShowFloat("Camera", "Fov", fov, 70.0f, 100.0f);
		ShowFloat("Camera", "Near", near_z, 70.0f, 100.0f);
		ShowFloat("Camera", "Far", far_z, 70.0f, 100.0f);
		ShowInt("Camera", "Index", camera_index, 70.0f, 100.0f);

		camera->SetSize(size);
		camera->SetFov(Math::ToRadian(fov));
		camera->SetNearZ(near_z);
		camera->SetFarZ(far_z);
		camera->SetCameraIndex(camera_index);

		//Culling Layer
		ImGui::Text("Culling");
		ImGui::SameLine(100.0f); //같은 라인에서 80만큼 떨어져서 시작

		for (UINT i = 0; i < 32; ++i)
		{
			m_p_culling_layer_list->AddItem(std::to_string(i));
		}

		const auto& culling_layer_list_vector = m_p_culling_layer_list->GetItemList();

		ImGui::PushItemWidth(60.0f);
		if (ImGui::BeginCombo("##CullingLayer", nullptr))
		{
			for (UINT i = 0; i < static_cast<UINT>(culling_layer_list_vector.size()); ++i)
			{
				UINT layer = 0;
				UINT bit_pos = static_cast<UINT>(std::stoi(culling_layer_list_vector[i]));

				layer = 1 << bit_pos;

				const bool is_selected = (culling_layer & layer);
				if (ImGui::Selectable(culling_layer_list_vector[i].c_str(), is_selected))
				{
					camera->CullingLayer(bit_pos);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		m_p_culling_layer_list->ClearItemList();

		ImGui::SameLine();

		if (ImGui::Button("Nothing", ImVec2(80.0f, 0.0f)))
		{
			camera->CullingNothing();
		}

		ImGui::SameLine();

		if (ImGui::Button("Everything", ImVec2(80.0f, 0.0f)))
		{
			camera->CullingEverything();
		}

		DrawComponentEnd();
	}
}

void GUI_Camera::SetProjectionType(Camera* camera, const std::string& projection_type_str)
{
	if (projection_type_str == "Orthographic")
		camera->SetProjectionType(ProjectionType::Orthographic);

	else if (projection_type_str == "Perspective")
		camera->SetProjectionType(ProjectionType::Perspective);
}
