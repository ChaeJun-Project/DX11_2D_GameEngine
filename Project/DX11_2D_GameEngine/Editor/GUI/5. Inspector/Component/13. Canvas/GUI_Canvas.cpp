#include "stdafx.h"
#include "GUI_Canvas.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include <DX11_2D_GameEngine_Lib/Scene.h>
#include <DX11_2D_GameEngine_Lib/Layer.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Canvas.h>

GUI_Canvas::GUI_Canvas(const std::string& canvas_gui_name)
	:GUI_Component(canvas_gui_name)
{
	m_p_camera_object_list = new GUI_ItemList();
}

GUI_Canvas::~GUI_Canvas()
{
	SAFE_DELETE(m_p_camera_object_list);
}

void GUI_Canvas::Render()
{
	//Canvas
	auto p_canvas = m_p_selected_game_object->GetComponent<Canvas>();
	if (p_canvas == nullptr)
		return;

	auto is_active = p_canvas->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::Canvas, is_active, IconType::Component_Canvas))
	{
		auto p_canvas = m_p_selected_game_object->GetComponent<Canvas>();
		if (p_canvas == nullptr)
			return;

		p_canvas->SetIsActive(is_active);

		//Property
		auto ui_camera_object_name = p_canvas->GetUICameraObjectName();

		//UI Camera
		ImGui::Text("UI Camera");
		ImGui::SameLine(80.0f);

		ImGui::PushItemWidth(200.0f);
		{
			int index = 0;

			const auto& camera_object_vector = SCENE_MANAGER->GetCurrentScene()->GetLayer(1)->GetGameObjects();

			for (const auto& camera_object : camera_object_vector)
			{
				m_p_camera_object_list->AddItem(camera_object->GetGameObjectName());

				if (camera_object->GetGameObjectName()._Equal(ui_camera_object_name))
					m_p_camera_object_list->SetCurrentListID(index);

				else
					++index;
			}

			const auto& camera_object_list_vector = m_p_camera_object_list->GetItemList();

			if (ImGui::BeginCombo("##UI Camera Name", ui_camera_object_name.c_str()))
			{
				for (UINT i = 0; i < static_cast<UINT>(camera_object_list_vector.size()); ++i)
				{
					const bool is_selected = (m_p_camera_object_list->GetCurrentListID() == i);
					if (ImGui::Selectable(camera_object_list_vector[i].c_str(), is_selected))
					{
						if (CAN_EDIT)
						{
							auto p_ui_camera_object = SCENE_MANAGER->GetCurrentScene()->FindGameObjectWithName(camera_object_list_vector[i]);
							//해당 GameObject가 Camera Component를 소유하고 있는 경우
							if (p_ui_camera_object->GetComponent<Camera>() != nullptr)
								p_canvas->SetUICameraObjectName(p_ui_camera_object->GetGameObjectName());

						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
		}
		ImGui::PopItemWidth();

		m_p_camera_object_list->ClearItemList();

		DrawComponentEnd();
	}
}
