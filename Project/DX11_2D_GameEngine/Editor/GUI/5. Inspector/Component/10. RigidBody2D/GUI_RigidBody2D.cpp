#include "stdafx.h"
#include "GUI_RigidBody2D.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/RigidBody2D.h>

GUI_RigidBody2D::GUI_RigidBody2D(const std::string& rigidbody2D_gui_name)
	:GUI_Component(rigidbody2D_gui_name)
{
}

void GUI_RigidBody2D::Render()
{
	//RigidBody2D
	auto p_rigidbody2D = m_p_selected_game_object->GetComponent<RigidBody2D>();
	if (p_rigidbody2D == nullptr)
		return;

	auto is_active = p_rigidbody2D->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::RigidBody2D, is_active, IconType::Component_RigidBody2D))
	{
		auto p_rigidbody2D = m_p_selected_game_object->GetComponent<RigidBody2D>();
		if (p_rigidbody2D == nullptr)
			return;

		p_rigidbody2D->SetIsActive(is_active);

		//Property
		auto gravity = p_rigidbody2D->GetGravity();
		auto mass = p_rigidbody2D->GetMass();

		//Gravity
		ImGui::Text("Gravity");
		ImGui::SameLine(60.0f);
		ImGui::PushItemWidth(100.0f);
		if (ImGui::InputFloat("##RigidBody2D Gravity", &gravity))
		{
			if (CAN_EDIT)
				p_rigidbody2D->SetGravity(gravity);
		}
		ImGui::PopItemWidth();
		
		//Mass
		ImGui::Text("Mass");
		ImGui::SameLine(60.0f);
		ImGui::PushItemWidth(100.0f);
		if (ImGui::InputFloat("##RigidBody2D Mass", &mass))
		{
			if (CAN_EDIT)
				p_rigidbody2D->SetMass(mass);
		}
		ImGui::PopItemWidth();

		DrawComponentEnd();
	}
}
