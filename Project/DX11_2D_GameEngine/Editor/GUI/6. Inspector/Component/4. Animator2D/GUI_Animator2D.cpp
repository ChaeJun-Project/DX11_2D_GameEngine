#include "stdafx.h"
#include "GUI_Animator2D.h"

#include "Helper/IconProvider.h"

#include "GUI/GUI_ItemList.h"

#include "Sprite Editor/GUI_SpriteEditor.h"

#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/Animation2D.h>

GUI_Animator2D::GUI_Animator2D(const std::string& animator_gui_name)
	:GUI_Component(animator_gui_name)
{
	m_p_item_list = new GUI_ItemList;
	m_p_gui_sprite_editor = new GUI_SpriteEditor;
}

GUI_Animator2D::~GUI_Animator2D()
{
	SAFE_DELETE(m_p_item_list);
	SAFE_DELETE(m_p_gui_sprite_editor);
}

void GUI_Animator2D::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::Animator2D, IconType::Component_Animator2D))
	{
		auto animator2D = m_select_game_object->GetComponent<Animator2D>();

		auto animator2D_index = animator2D->GetAnimator2DID();
		auto animation_play_speed = animator2D->GetAnimationSpeed();

		//Sprite Editor
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 110.0f);
		if (ImGui::Button("Sprite Editor", ImVec2(110.0f, 0.0f)))
		{
			m_p_gui_sprite_editor->m_is_active = true;
			m_p_gui_sprite_editor->m_p_current_animater = animator2D;
		}

		//Animator Index
		ShowInt("Animator2D", "Index", animator2D_index, 100.0f);

		//Current Animation
		ImGui::Text("Current Animation2D");
		ImGui::SameLine();

		//Show Combo
		ImGui::PushItemWidth(200.0f);
		{
			int index = 0;

			const auto& animation_map = animator2D->GetAnimationMap();
			std::string current_animation_name;
			auto current_animation = animator2D->GetCurrentAnimation();

			//현재 적용 중인 애니메이션이 있다면
			if (current_animation != nullptr)
				current_animation_name = current_animation->GetAnimationName();

			for (auto& animation : animation_map)
			{
				m_p_item_list->AddItem(animation.second->GetAnimationName());

				if (!current_animation_name.empty() &&
					animation.second->GetAnimationName() == current_animation_name)
					m_p_item_list->SetCurrentListID(index);

				else
					++index;
			}

			const auto& item_list_vector = m_p_item_list->GetItemList();

			if (ImGui::BeginCombo("##Animation2D List", current_animation_name.c_str()))
			{
				for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
				{
					const bool is_selected = (*(m_p_item_list->GetCurrentListID()) == i);
					if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
					{
						animator2D->Stop();
						animator2D->SetCurrentAnimation(item_list_vector[i]);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			//Sprite Editor가 꺼져있다면 클리어
			if (!m_p_gui_sprite_editor->m_is_active)
				m_p_item_list->ClearItemList();

			ImGui::PopItemWidth();
		}

		//Animator Play Reverse Check
		//버그있음
		ImGui::Checkbox("##Animator2D Play Reverse", &animator2D->GetIsPlayReverse());
		ImGui::SameLine();
		ImGui::Text("Play Reverse");

		//Animator Play Loop
		ImGui::Checkbox("##Animator2D Play Loop", &animator2D->GetIsLoop());
		ImGui::SameLine();
		ImGui::Text("Play Loop");

		//Animator Play Speed
		ShowFloat("Animator2D", "Play Speed", animation_play_speed, 100.0f);

		//Set Animator Property
		animator2D->SetAnimator2DID(animator2D_index);
		animator2D->SetAnimationSpeed(animation_play_speed);

		DrawComponentEnd();
	}

	if (m_p_gui_sprite_editor->m_is_active)
	{
		m_p_gui_sprite_editor->Render();
		m_p_gui_sprite_editor->m_p_animation_item_list = m_p_item_list;
	}
}
