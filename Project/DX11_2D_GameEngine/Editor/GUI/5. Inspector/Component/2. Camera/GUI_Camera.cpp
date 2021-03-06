#include "stdafx.h"
#include "GUI_Camera.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>

GUI_Camera::GUI_Camera(const std::string& camera_gui_name)
	:GUI_Component(camera_gui_name)
{
	m_p_projection_list = new GUI_ItemList();
	m_p_culling_layer_list = new GUI_ItemList();
}

GUI_Camera::~GUI_Camera()
{
	SAFE_DELETE(m_p_projection_list);
	SAFE_DELETE(m_p_culling_layer_list);
}

void GUI_Camera::Render()
{
	auto p_camera = m_p_selected_game_object->GetComponent<Camera>();
	if (p_camera == nullptr)
		return;

	auto is_active = p_camera->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::Camera, is_active, IconType::Component_Camera))
	{
		auto p_camera = m_p_selected_game_object->GetComponent<Camera>();
		if (p_camera == nullptr)
			return;

		p_camera->SetIsActive(is_active);
		
		//Camera Property
		auto projection_type = p_camera->GetProjectionType();
		auto size = p_camera->GetSize();
		auto fov = Math::ToDegree(p_camera->GetFov());
		auto near_z = p_camera->GetNearZ();
		auto far_z = p_camera->GetFarZ();
		auto camera_index = p_camera->GetCameraIndex();
		auto culling_layer = p_camera->GetCullingLayer();

		//Projection
		ShowComboProjectionType(p_camera, projection_type);

		switch (p_camera->GetProjectionType())
		{
		case ProjectionType::Orthographic:
			ShowFloat2("Size", size, 100.0f, 100.0f);
			break;
		case ProjectionType::Perspective:

			break;
		}

		//fov, near_z, far_z, Camera_Index
		ShowFloat("Camera", "Fov", fov, 100.f, 100.0f);
		ShowFloat("Camera", "Near", near_z, 100.f, 100.0f);
		ShowFloat("Camera", "Far", far_z, 100.f, 100.0f);
		ShowInt("Camera", "Index", camera_index, 100.f, 100.0f);

		if (CAN_EDIT)
		{
			p_camera->SetSize(size);
			p_camera->SetFov(Math::ToRadian(fov));
			p_camera->SetNearZ(near_z);
			p_camera->SetFarZ(far_z);
			p_camera->SetCameraIndex(camera_index);
		}

		//Culling Layer
		ImGui::Text("Culling");
		ImGui::SameLine(100.0f); //???? ???????? 80???? ???????? ????

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
					if (CAN_EDIT)
						p_camera->CullingLayer(bit_pos);
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
			if (CAN_EDIT)
				p_camera->CullingNothing();
		}

		ImGui::SameLine();

		if (ImGui::Button("Everything", ImVec2(80.0f, 0.0f)))
		{
			if (CAN_EDIT)
				p_camera->CullingEverything();
		}

		DrawComponentEnd();
	}
}

void GUI_Camera::ShowComboProjectionType(Camera* p_camera, ProjectionType& projection_type)
{
	ImGui::BeginGroup();
	ImGui::Text("Projection");
	ImGui::SameLine(90.0f); //???? ???????? 80???? ???????? ????

	//Show Combo
	ImGui::PushItemWidth(120.0f);
	{
		int index = 0;

		std::string current_projection_type;

		switch (projection_type)
		{
		case ProjectionType::Orthographic:
			current_projection_type = "Orthographic";
			break;
		case ProjectionType::Perspective:
			current_projection_type = "Perspective";
			break;
		}

		m_p_projection_list->AddItem("Orthographic");
		m_p_projection_list->AddItem("Perspective");

		const auto& projection_list_vector = m_p_projection_list->GetItemList();

		for (auto& projection_type : projection_list_vector)
		{
			if (projection_type == current_projection_type)
				m_p_projection_list->SetCurrentListID(index);

			else
				++index;
		}

		if (ImGui::BeginCombo("##Projection Type", current_projection_type.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(projection_list_vector.size()); ++i)
			{
				const bool is_selected = (m_p_projection_list->GetCurrentListID() == i);
				if (ImGui::Selectable(projection_list_vector[i].c_str(), is_selected))
				{
					p_camera->SetProjectionType(static_cast<ProjectionType>(i));
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		m_p_projection_list->ClearItemList();
	}
	ImGui::PopItemWidth();
	ImGui::EndGroup();
}