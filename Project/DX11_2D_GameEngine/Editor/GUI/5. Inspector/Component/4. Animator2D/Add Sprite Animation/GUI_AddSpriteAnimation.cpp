#include "stdafx.h"
#include "GUI_AddSpriteAnimation.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>

#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

GUI_AddSpriteAnimation::GUI_AddSpriteAnimation()
{
	m_p_sprite_animation_item_list = std::make_unique<GUI_ItemList>();
	m_p_added_sprite_animation_item_list = std::make_unique<GUI_ItemList>();
}

GUI_AddSpriteAnimation::~GUI_AddSpriteAnimation()
{
	p_current_animator2D = nullptr;

	m_p_sprite_animation_item_list.reset();
	m_p_added_sprite_animation_item_list.reset();
}

void GUI_AddSpriteAnimation::Initialize()
{
	p_current_animator2D = nullptr;

	m_p_sprite_animation_item_list->ClearItemList();
	m_p_added_sprite_animation_item_list->ClearItemList();
}

void GUI_AddSpriteAnimation::Render()
{
	if (ImGui::Begin("Add Sprite Animation", &m_is_active))
	{
		auto resource_manager = RESOURCE_MANAGER;

		//Sprite Animation List
		ImGui::BulletText("Sprite Animation List");
		ImGui::SameLine(230.0f);

		const auto& sprite_animation_map = resource_manager->GetResourceMap(ResourceType::SpriteAnimation);
		//현재 참조하고 있는 Animator2D의 Sprite Animation Map을 참조
		auto& animator2D_sprite_animation_map = p_current_animator2D->GetAnimationMap();

		for (const auto& sprite_animation : sprite_animation_map)
			m_p_sprite_animation_item_list->AddItem(sprite_animation.second->GetResourceName());

		const auto& sprite_animation_item_list_vector = m_p_sprite_animation_item_list->GetItemList();

		//Show Sprite Animation List Combo
		ImGui::PushItemWidth(150.0f);
		if (ImGui::BeginCombo("##Sprite Animation List", ""))
		{
			for (UINT i = 0; i < static_cast<UINT>(sprite_animation_item_list_vector.size()); ++i)
			{
				const bool is_selected = p_current_animator2D->HasAnimation(sprite_animation_item_list_vector[i]);
				if (ImGui::Selectable(sprite_animation_item_list_vector[i].c_str(), is_selected))
				{
					p_current_animator2D->AddAnimation(sprite_animation_item_list_vector[i]);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		m_p_sprite_animation_item_list->ClearItemList();
		ImGui::PopItemWidth();

		//현재 참조하고 있는 Animator2D에 추가된 Sprite Animation 리스트
		//Current Animator2D Sprite Animation List
		ImGui::BulletText("Added Sprite Animation List");
		ImGui::SameLine(230.0f);

		for (const auto& sprite_animation : animator2D_sprite_animation_map)
			m_p_added_sprite_animation_item_list->AddItem(sprite_animation.second->GetResourceName());

		const auto& added_sprite_animation_item_list_vector = m_p_added_sprite_animation_item_list->GetItemList();

		//Show Added Sprite Animation List Combo
		ImGui::PushItemWidth(150.0f);
		if (ImGui::BeginCombo("##Added Sprite Animation List", ""))
		{
			for (UINT i = 0; i < static_cast<UINT>(added_sprite_animation_item_list_vector.size()); ++i)
			{
				if (ImGui::Selectable(added_sprite_animation_item_list_vector[i].c_str(), false))
				{
					p_current_animator2D->DeleteAnimation(added_sprite_animation_item_list_vector[i]);
				}
			}

			ImGui::EndCombo();
		}
		m_p_added_sprite_animation_item_list->ClearItemList();
		ImGui::PopItemWidth();

		ImGui::End();
	}
}
