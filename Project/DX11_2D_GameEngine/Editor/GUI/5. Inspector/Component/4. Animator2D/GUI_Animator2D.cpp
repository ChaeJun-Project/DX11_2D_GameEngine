#include "stdafx.h"
#include "GUI_Animator2D.h"

#include "Helper/IconProvider.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

GUI_Animator2D::GUI_Animator2D(const std::string& animator_gui_name)
	:GUI_Component(animator_gui_name)
{
	m_p_sprite_animation_list = std::make_unique<GUI_ItemList>();
}

GUI_Animator2D::~GUI_Animator2D()
{
	m_p_sprite_animation_list.reset();
}

void GUI_Animator2D::Render()
{
	auto animator2D = m_p_selected_game_object->GetComponent<Animator2D>();
	if (animator2D == nullptr)
		return;

	auto is_active = animator2D->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::Animator2D, is_active, IconType::Component_Animator2D))
	{
		auto animator2D = m_p_selected_game_object->GetComponent<Animator2D>();
		if (animator2D == nullptr)
			return;

		animator2D->SetIsActive(is_active);
		
		auto is_play_reverse = animator2D->GetIsPlayReverse();
		auto is_loop = animator2D->GetIsLoop();
		auto animation_play_speed = animator2D->GetAnimationSpeed();

		//TODO: Script까지 구현되었을 때 풀어줘야 함
		//Current Sprtie Animation
		//ImGui::Text("Current SpriteAnimation");
		//ImGui::SameLine();
		//auto current_sprite_animation = animator2D->GetCurrentAnimation();
		//auto current_sprite_animation_name = current_sprite_animation->GetResourceName();
		//ImGui::InputText("##Current Sprtie Animation", &current_sprite_animation_name, ImGuiInputTextFlags_ReadOnly);


		//Sprite Animation List
		//현재 참조하고 있는 Animator2D에 추가된 Sprite Animation 리스트
		//Current Animator2D Sprite Animation List
		ImGui::Text("Sprite Animation List");
		ImGui::SameLine();

		//현재 참조하고 있는 Animator2D의 Sprite Animation Map을 참조
		auto& animator2D_sprite_animation_map = animator2D->GetAnimationMap();

		auto current_animation = animator2D->GetCurrentAnimation();
		std::string current_animation_name;
		if (current_animation)
			current_animation_name = current_animation->GetResourceName();

		int index = 0;

		for (const auto& sprite_animation : animator2D_sprite_animation_map)
		{
			m_p_sprite_animation_list->AddItem(sprite_animation.second->GetResourceName());

			if (current_animation)
			{
				if (sprite_animation.second->GetResourceName() == current_animation_name)
					m_p_sprite_animation_list->SetCurrentListID(index);

				else
					++index;
			}
		}

		const auto& sprite_animation_item_list_vector = m_p_sprite_animation_list->GetItemList();

		//Show Added Sprite Animation List Combo
		ImGui::PushItemWidth(150.0f);
		if (ImGui::BeginCombo("##Sprite Animation List", current_animation_name.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(sprite_animation_item_list_vector.size()); ++i)
			{
				const bool is_selected = (m_p_sprite_animation_list->GetCurrentListID() == i);
				if (ImGui::Selectable(sprite_animation_item_list_vector[i].c_str(), is_selected))
				{
					animator2D->SetCurrentAnimation(sprite_animation_item_list_vector[i]);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		m_p_sprite_animation_list->ClearItemList();
		ImGui::PopItemWidth();
		if (CAN_EDIT)
		{
			if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Animation))
			{
				auto p_sprite_animation = RESOURCE_MANAGER->LoadFromFile<SpriteAnimation>(std::get<std::string>(pay_load->data));
				animator2D->AddAnimation(p_sprite_animation->GetResourceName());
			}
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
			animator2D->SetAnimationSpeed(animation_play_speed);
		}

		DrawComponentEnd();
	}
}
