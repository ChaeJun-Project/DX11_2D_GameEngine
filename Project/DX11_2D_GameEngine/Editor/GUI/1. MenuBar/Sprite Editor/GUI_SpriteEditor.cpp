#include "stdafx.h"
#include "GUI_SpriteEditor.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include "GUI_SpritePlayer.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>

#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

GUI_SpriteEditor::GUI_SpriteEditor()
{
	m_p_atlas_item_list = new GUI_ItemList();
	m_p_sprtie_animation_item_list = new GUI_ItemList();
	m_p_gui_sprite_player = new GUI_SpritePlayer();
}

GUI_SpriteEditor::~GUI_SpriteEditor()
{
	//Atlas Texture
	m_p_atlas_texture.reset();
	SAFE_DELETE(m_p_atlas_item_list);

	//SpriteAnimation
	m_p_sprite_animation.reset();
	SAFE_DELETE(m_p_sprtie_animation_item_list);

	m_draw_list = nullptr;

	//Sprite Player
	SAFE_DELETE(m_p_gui_sprite_player);
}

void GUI_SpriteEditor::Initialize()
{
	//Atlas Texture
	m_p_atlas_texture.reset();
	m_atlas_texture_size = Vector2::Zero;

	//Atlas Texture List
	m_p_atlas_item_list->ClearItemList();
	//Sprite Animation List
	m_p_sprtie_animation_item_list->ClearItemList();

	//Sprite Animation
	m_is_create_new_sprite_animation = false;
	m_animation_name.clear();
	m_p_sprite_animation.reset();
	m_frame_index = 0;
	m_total_frame_count = 0;

	//Draw Rect
	m_draw_list = nullptr;
	m_canvas_left_top = ImVec2(0.0f, 0.0f);
	m_canvas_right_bottom = ImVec2(0.0f, 0.0f);

	m_mouse_pos_in_canvas = ImVec2(0.0f, 0.0f);

	m_start_point_in_canvas = ImVec2(0.0f, 0.0f);
	m_end_point_in_canvas = ImVec2(0.0f, 0.0f);

	m_draw_left_top = Vector2(0.0f, 0.0f);
	m_draw_right_bottom = Vector2(0.0f, 0.0f);
}

void GUI_SpriteEditor::Render()
{
	if (ImGui::Begin("Sprite Editor", &m_is_active))
	{
		auto resource_manager = ResourceManager::GetInstance();

		ImGui::BulletText("Atlas Texture");
		ImGui::SameLine();

		//Show Altas Texture Combo
		ImGui::PushItemWidth(150.0f);
		{
			int index = 0;

			const auto& atlas_texture_map = resource_manager->GetResourceMap(ResourceType::Texture);

			std::string current_atlas_name;

			if (m_p_atlas_texture != nullptr)
			{
				current_atlas_name = m_p_atlas_texture->GetResourceName();
			}

			for (auto& atlas_texture : atlas_texture_map)
			{
				m_p_atlas_item_list->AddItem(atlas_texture.second->GetResourceName());

				if (!current_atlas_name.empty() &&
					atlas_texture.second->GetResourceName() == current_atlas_name)
					m_p_atlas_item_list->SetCurrentListID(index);

				else
					++index;
			}

			const auto& item_list_vector = m_p_atlas_item_list->GetItemList();

			if (ImGui::BeginCombo("##Atlas Texture List", current_atlas_name.c_str()))
			{
				for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
				{
					const bool is_selected = (*(m_p_atlas_item_list->GetCurrentListID()) == i);
					if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
					{
						m_p_atlas_texture = resource_manager->GetResource<Texture>(item_list_vector[i]);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			m_p_atlas_item_list->ClearItemList();

			ImGui::PopItemWidth();
		}
		ImGui::SameLine();

		ImGui::BulletText("Sprite Animation");
		ImGui::SameLine();

		//Show Sprite Animation Combo
		ImGui::PushItemWidth(150.0f);
		{
			int index = 0;

			const auto& animation2D_map = resource_manager->GetResourceMap(ResourceType::SpriteAnimation);

			std::string current_sprite_animation_name;

			if (m_p_sprite_animation != nullptr)
			{
				current_sprite_animation_name = m_p_sprite_animation->GetResourceName();
			}

			for (auto& sprite_animation : animation2D_map)
			{
				m_p_sprtie_animation_item_list->AddItem(sprite_animation.second->GetResourceName());

				if (!current_sprite_animation_name.empty() &&
					sprite_animation.second->GetResourceName() == current_sprite_animation_name)
					m_p_sprtie_animation_item_list->SetCurrentListID(index);

				else
					++index;
			}

			const auto& item_list_vector = m_p_sprtie_animation_item_list->GetItemList();

			if (ImGui::BeginCombo("##Sprite Animation List", current_sprite_animation_name.c_str()))
			{
				for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
				{
					const bool is_selected = (*(m_p_sprtie_animation_item_list->GetCurrentListID()) == i);
					if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
					{
						m_p_sprite_animation = resource_manager->GetResource<SpriteAnimation>(item_list_vector[i]);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			m_p_sprtie_animation_item_list->ClearItemList();

			ImGui::PopItemWidth();
		}
		ImGui::SameLine();

		//Show Save Sprite Animation Button
		if (ImGui::Button("Save Sprite Animation", ImVec2(170.0f, 0.0f)))
		{
			if (m_p_sprite_animation != nullptr)
				FileFunction::SaveFile(ANIMATION_PATH, m_p_sprite_animation->GetResourceName(), FileType::Animation);
		}
		ImGui::SameLine();

		//Show Load Sprite Animation Button
		if (ImGui::Button("Load Sprite Animation", ImVec2(170.0f, 0.0f)))
		{
			std::string animation2D_path = FileFunction::LoadFile(ANIMATION_PATH, FileType::Animation);

			if (!animation2D_path.empty())
			{
				Initialize();
				m_p_sprite_animation = resource_manager->LoadFromFile<SpriteAnimation>(animation2D_path);

				if (m_p_sprite_animation != nullptr)
				{
					m_p_atlas_texture = m_p_sprite_animation->GetAtlasTexture();
					m_is_create_new_sprite_animation = true;
					m_animation_name = m_p_sprite_animation->GetResourceName();
				}
			}
		}

		ImGui::Separator();

		if (m_p_atlas_texture != nullptr)
		{
			m_canvas_left_top = ImGui::GetCursorScreenPos();
			m_canvas_right_bottom = m_canvas_left_top + ImVec2(m_atlas_texture_size.x, m_atlas_texture_size.y);

			ImGui::BeginGroup();

			//Show Atlas Texture
			ImGui::Image
			(
				m_p_atlas_texture->GetShaderResourceView(),
				ImVec2(m_atlas_texture_size.x, m_atlas_texture_size.y),
				ImVec2(0.0f, 0.0f),
				ImVec2(1.0f, 1.0f),
				ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
				ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
			);

			//Show Atlas Texture Info
			m_atlas_texture_size = Vector2::Zero;
			m_atlas_texture_size = Vector2(static_cast<float>(m_p_atlas_texture->GetWidth()), static_cast<float>(m_p_atlas_texture->GetHeight()));

			std::string atlas_texture_name = m_p_atlas_texture->GetResourceName() + "(" + std::to_string(m_p_atlas_texture->GetWidth()) + " X " + std::to_string(m_p_atlas_texture->GetHeight()) + ")";
			ImGui::Text(atlas_texture_name.c_str());

			const bool is_hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held

			//Show Preview Sprite Animation Button
			if (ImGui::Button("Preview Sprite Animation", ImVec2(200.0f, 0.0f)))
			{
				if (m_p_sprite_animation != nullptr && m_p_sprite_animation->HasAnimationFrame())
				{
					if (!m_p_gui_sprite_player->m_is_active)
					{
						m_p_gui_sprite_player->Initialize();
						m_p_gui_sprite_player->m_p_current_animation2D = m_p_sprite_animation;
						m_p_gui_sprite_player->m_is_active = true;
					}
				}
			}

			ImGui::EndGroup();
			ImGui::SameLine();

			ShowAnimationEditor();

			DrawRect(is_hovered, is_active);
		}

		if (m_p_gui_sprite_player->m_is_active)
			m_p_gui_sprite_player->Render();

		ImGui::End();
	}
}

void GUI_SpriteEditor::ShowAnimationEditor()
{
	if (m_p_atlas_texture == nullptr)
		return;

	ImGui::BeginGroup();

	//Create Animation Button
	if (ImGui::Button("Create New Sprite Animation", ImVec2(200.0f, 0.0f)))
	{
		if (m_is_create_new_sprite_animation)
		{
			if (m_p_sprite_animation != nullptr)
			{
				m_p_sprite_animation.reset();
				m_animation_name.clear();
			}
		}

		m_is_create_new_sprite_animation = true;
	}

	if (m_is_create_new_sprite_animation)
	{
		ImGui::BulletText("Sprite Animation Name");
		ImGui::SameLine();
		ImGui::PushItemWidth(150.0f);
		if (ImGui::InputText("##Sprite Animation Name", &m_animation_name, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (m_animation_name.size())
			{
				m_p_sprite_animation = ResourceManager::GetInstance()->CreateSpriteAnimation(m_animation_name); //Game Object의 이름을 수정한 경우에만 수행
				m_p_sprite_animation->SetAtlasTexture(m_p_atlas_texture);
			}
		}
		ImGui::PopItemWidth();

		//성공적으로 애니메이션이 만들어졌다면
		if (m_p_sprite_animation != nullptr)
		{
			ImGui::BulletText("Animation Frame ID");

			SpriteAnimation_Frame animation2D_frame;
			ZeroMemory(&animation2D_frame, sizeof(SpriteAnimation_Frame));

			if (m_p_sprite_animation->HasAnimationFrame())
			{
				animation2D_frame = m_p_sprite_animation->GetAnimationFrame(static_cast<UINT>(m_frame_index));

				m_total_frame_count = m_p_sprite_animation->GetAnimationFrameCount() - 1;

				//Frame Index(Frame Data가 1개라도 존재한느 경우에만 렌더링)
				ImGui::SameLine();
				ImGui::PushItemWidth(100.0f);
				static int slider_index = 0;
				if (ImGui::SliderInt("", &slider_index, 0, m_total_frame_count))
				{
					m_frame_index = slider_index;
					animation2D_frame = m_p_sprite_animation->GetAnimationFrame(static_cast<UINT>(m_frame_index));
				}
				ImGui::PopItemWidth();

				//Left Top
				ShowFloat2("Left Top", animation2D_frame.animation2D_data.left_top, 70.0f, 150.0f);

				//Frame Size
				ShowFloat2("Frame Size", animation2D_frame.animation2D_data.frame_size, 70.0f, 150.0f);

				//Full Frame Size
				ShowFloat2("Full Frame Size", animation2D_frame.animation2D_data.full_frame_size, 70.0f, 150.0f);

				//Offset
				ShowFloat2("Offset", animation2D_frame.animation2D_data.offset, 70.0f, 150.0f);

				//Duration
				ShowFloat(m_p_sprite_animation->GetResourceName(), "Duration", animation2D_frame.duration, 150.0f);


				m_p_sprite_animation->SetAnimationFrame(m_frame_index, animation2D_frame);

				SpriteAnimation_Data animation2D_data = animation2D_frame.animation2D_data;

				Vector2 uv_left_top = Vector2
				(
					animation2D_data.left_top.x / m_atlas_texture_size.x + animation2D_data.offset.x / animation2D_data.full_frame_size.x,
					animation2D_data.left_top.y / m_atlas_texture_size.y + animation2D_data.offset.y / animation2D_data.full_frame_size.y
				);
				Vector2 uv_right_bottom = Vector2
				(
					(animation2D_data.left_top.x + animation2D_data.full_frame_size.x) / m_atlas_texture_size.x + animation2D_data.offset.x / animation2D_data.full_frame_size.x,
					(animation2D_data.left_top.y + animation2D_data.full_frame_size.y) / m_atlas_texture_size.y + animation2D_data.offset.y / animation2D_data.full_frame_size.y
				);

				//Preview Animation
				ImVec2 preview_rect_size = ImVec2(animation2D_frame.animation2D_data.full_frame_size.x * 2.0f, animation2D_frame.animation2D_data.full_frame_size.y * 2.0f);
				ImVec2 frame_rect_size = ImVec2(animation2D_frame.animation2D_data.frame_size.x * 2.0f, animation2D_frame.animation2D_data.frame_size.y * 2.0f);

				ImGui::BulletText("Preview Frame");

				ImVec2 current_screen_pos = ImGui::GetCursorScreenPos();

				if (preview_rect_size.x > 0.0f && preview_rect_size.y > 0.0f)
				{
					ImGui::Image
					(
						m_p_atlas_texture->GetShaderResourceView(),
						preview_rect_size,
						ImVec2(uv_left_top.x, uv_left_top.y),
						ImVec2(uv_right_bottom.x, uv_right_bottom.y),
						ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
						ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
					);
				}

				ImVec2 preview_frame_rect_left_top(0.0f, 0.0f);
				ImVec2 preview_frame_rect_right_bottom(0.0f, 0.0f);

				if (animation2D_data.frame_size.x <= animation2D_data.full_frame_size.x &&
					animation2D_data.frame_size.y <= animation2D_data.full_frame_size.y)
				{
					preview_frame_rect_left_top = ImVec2
					(
						current_screen_pos.x + ((preview_rect_size.x - frame_rect_size.x) / 2) + 1.0f,
						current_screen_pos.y + ((preview_rect_size.y - frame_rect_size.y) / 2) + 1.0f
					);

					preview_frame_rect_right_bottom = ImVec2
					(
						preview_frame_rect_left_top.x + frame_rect_size.x - 1.0f,
						preview_frame_rect_left_top.y + frame_rect_size.y - 1.0f
					);
				}

				m_draw_list = ImGui::GetWindowDrawList();
				//Canvas의 범위
				m_draw_list->PushClipRect(current_screen_pos, current_screen_pos + preview_rect_size, true);
				//Vertical Cross
				m_draw_list->AddLine
				(
					ImVec2(current_screen_pos.x + (preview_rect_size.x / 2), current_screen_pos.y + 1.0f),
					ImVec2(current_screen_pos.x + (preview_rect_size.x / 2), current_screen_pos.y + preview_rect_size.y + 1.0f),
					IM_COL32(255, 0, 0, 200),
					3.0f
				);

				//Horizontal Cross
				m_draw_list->AddLine
				(
					ImVec2(current_screen_pos.x + 1.0f, current_screen_pos.y + (preview_rect_size.y / 2)),
					ImVec2(current_screen_pos.x + preview_rect_size.x + 1.0f, current_screen_pos.y + (preview_rect_size.y / 2)),
					IM_COL32(0, 0, 255, 200),
					3.0f
				);

				//Frame Rect
				m_draw_list->AddRect(ImVec2(preview_frame_rect_left_top.x, preview_frame_rect_left_top.y), ImVec2(preview_frame_rect_right_bottom.x, preview_frame_rect_right_bottom.y), IM_COL32(0, 255, 0, 255));
				m_draw_list->PopClipRect();
			}

			if (ImGui::Button("Create Frame", ImVec2(120.0f, 0.0f)))
			{
				ZeroMemory(&animation2D_frame, sizeof(SpriteAnimation_Frame));
				m_p_sprite_animation->CreateAnimationFrame(animation2D_frame);
				m_start_point_in_canvas = ImVec2(0.0f, 0.0f);
				m_end_point_in_canvas = ImVec2(0.0f, 0.0f);
			}

			if (ImGui::Button("Delete Frame", ImVec2(120.0f, 0.0f)))
			{
				m_frame_index = m_p_sprite_animation->DeleteAnimationFrame(m_frame_index);
			}
		}
	}

	ImGui::EndGroup();
}

void GUI_SpriteEditor::DrawRect(const bool& is_hovered, const bool& is_active)
{
	if (m_p_sprite_animation == nullptr)
		return;

	if (m_p_sprite_animation->HasAnimationFrame())
	{
		//Canvas의 시작점
		const ImVec2 origin(m_canvas_left_top.x, m_canvas_left_top.y);

		SpriteAnimation_Frame animation2D_frame = m_p_sprite_animation->GetAnimationFrame(static_cast<UINT>(m_frame_index));

		//Draw Rect
		m_draw_left_top = Vector2(animation2D_frame.animation2D_data.left_top.x, animation2D_frame.animation2D_data.left_top.y);
		m_draw_right_bottom = Vector2(m_draw_left_top.x + animation2D_frame.animation2D_data.full_frame_size.x, m_draw_left_top.y + animation2D_frame.animation2D_data.full_frame_size.y);

		//Canvas의 상대적인 좌표를 구함(= Atlas Texture 상에서의 left_top, right_bottom 좌표를 구함)
		m_draw_left_top.x += origin.x;
		m_draw_left_top.y += origin.y;

		m_draw_right_bottom.x += origin.x;
		m_draw_right_bottom.y += origin.y;

		//Canvas의 범위
		if (animation2D_frame.animation2D_data.full_frame_size.x != 0.0f || animation2D_frame.animation2D_data.full_frame_size.y != 0.0f)
		{
			m_draw_list->PushClipRect(origin, m_canvas_right_bottom, true);
			m_draw_list->AddRect(ImVec2(m_draw_left_top.x, m_draw_left_top.y), ImVec2(m_draw_right_bottom.x, m_draw_right_bottom.y), IM_COL32(0, 255, 0, 255));
			m_draw_list->PopClipRect();
		}
	}
}