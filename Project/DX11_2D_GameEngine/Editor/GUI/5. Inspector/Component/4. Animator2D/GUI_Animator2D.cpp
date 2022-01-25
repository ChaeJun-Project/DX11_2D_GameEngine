#include "stdafx.h"
#include "GUI_Animator2D.h"

#include "Helper/IconProvider.h"

#include "Add Sprite Animation/GUI_AddSpriteAnimation.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

GUI_Animator2D::GUI_Animator2D(const std::string& animator_gui_name)
	:GUI_Component(animator_gui_name)
{
	m_p_add_sprite_animation = std::make_unique<GUI_AddSpriteAnimation>();
}

GUI_Animator2D::~GUI_Animator2D()
{
	m_p_add_sprite_animation.reset();
}

void GUI_Animator2D::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Animator2D, IconType::Component_Animator2D))
	{
		auto animator2D = m_select_game_object->GetComponent<Animator2D>();
		if (animator2D == nullptr)
			return;

		auto animator2D_index = animator2D->GetAnimator2DID();
		auto is_play_reverse = animator2D->GetIsPlayReverse();
		auto is_loop = animator2D->GetIsLoop();
		auto animation_play_speed = animator2D->GetAnimationSpeed();

		//Add Sprite Animation
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 150.0f);
		if (ImGui::Button("Add Sprite Animation", ImVec2(150.0f, 0.0f)))
		{
			if (CAN_EDIT)
			{
				if (!m_p_add_sprite_animation->m_is_active)
				{
					m_p_add_sprite_animation->Initialize();
					m_p_add_sprite_animation->p_current_animator2D = animator2D;
					m_p_add_sprite_animation->m_is_active = true;
				}
			}
		}

		if (m_p_add_sprite_animation->m_is_active)
			m_p_add_sprite_animation->Render();


		if (CAN_EDIT)
			//Animator Index
			ShowInt("Animator2D", "Animator2D Index", animator2D_index, 50.0f, 150.0f);

		else
		{
		    //TODO: Script까지 구현되었을 때 풀어줘야 함
			//Current Sprtie Animation
			ImGui::Text("Current SpriteAnimation");
			//ImGui::SameLine();
			//auto current_sprite_animation = animator2D->GetCurrentAnimation();
			//auto current_sprite_animation_name = current_sprite_animation->GetResourceName();
			//ImGui::InputText("##Current Sprtie Animation", &current_sprite_animation_name, ImGuiInputTextFlags_ReadOnly);
		}

		//Animator Play Reverse Check
		if (ImGui::Checkbox("##Sprite Animation Play Reverse", &is_play_reverse))
		{
			if (CAN_EDIT)
				animator2D->SetIsPlayReverse(is_play_reverse);
		}
		ImGui::SameLine();
		ImGui::Text("Play Reverse");

		//Animator Play Loop
		if (ImGui::Checkbox("##Sprite Animation Play Loop", &is_loop))
		{
			if (CAN_EDIT)
				animator2D->SetIsLoop(is_loop);
		}
		ImGui::SameLine();
		ImGui::Text("Play Loop");

		//Animator Play Speed
		ShowFloat("Animator2D", "Play Speed", animation_play_speed, 100.0f);

		//Set Animator Property
		if (CAN_EDIT)
		{
			animator2D->SetAnimator2DID(animator2D_index);
			animator2D->SetAnimationSpeed(animation_play_speed);
		}

		DrawComponentEnd();
	}
}
