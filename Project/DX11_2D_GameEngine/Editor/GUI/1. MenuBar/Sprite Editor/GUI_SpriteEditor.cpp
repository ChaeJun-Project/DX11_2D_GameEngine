#include "stdafx.h"
#include "GUI_SpriteEditor.h"

#include "GUI_SpritePlayer.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>

#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

GUI_SpriteEditor::GUI_SpriteEditor()
{
	m_p_gui_sprite_player = new GUI_SpritePlayer();
}

GUI_SpriteEditor::~GUI_SpriteEditor()
{
	m_draw_list = nullptr;

	//Sprite Player
	SAFE_DELETE(m_p_gui_sprite_player);
}

void GUI_SpriteEditor::Initialize()
{
	//Atlas Texture
	m_p_atlas_texture.reset();
	m_atlas_texture_name.clear();
	m_atlas_texture_size = Vector2::Zero;

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
		//Atlas Texture
		ImGui::BulletText("Atlas Texture");
		ImGui::SameLine();

		ImGui::PushItemWidth(200.0f);
		ImGui::InputText("##Atlas Texture", &m_atlas_texture_name, ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Texture))
		{
			Initialize();
			m_p_atlas_texture = RESOURCE_MANAGER->LoadFromFile<Texture>(std::get<std::string>(pay_load->data));

			if (m_p_atlas_texture != nullptr)
			{
				m_atlas_texture_name = m_p_atlas_texture->GetResourceName();
			}
		}
		ImGui::SameLine();

		//Sprite Animation
		ImGui::BulletText("Sprite Animation");
		ImGui::SameLine();

		ImGui::PushItemWidth(200.0f);
		ImGui::InputText("##Sprite Animation", &m_animation_name, ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Animation))
		{
			Initialize();
			m_p_sprite_animation = RESOURCE_MANAGER->LoadFromFile<SpriteAnimation>(std::get<std::string>(pay_load->data));

			if (m_p_sprite_animation != nullptr)
			{
				m_p_atlas_texture = m_p_sprite_animation->GetAtlasTexture();
				m_atlas_texture_name = m_p_atlas_texture->GetResourceName();
				m_is_create_new_sprite_animation = true;
				m_animation_name = m_p_sprite_animation->GetResourceName();
			}
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
				m_p_sprite_animation = RESOURCE_MANAGER->LoadFromFile<SpriteAnimation>(animation2D_path);

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
			if (!m_animation_name.empty())
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
				if (ImGui::SliderInt("##Sprite Animation Index", &m_frame_index, 0, m_total_frame_count))
				{
					animation2D_frame = m_p_sprite_animation->GetAnimationFrame(static_cast<UINT>(m_frame_index));
				}
				ImGui::PopItemWidth();

				//Left Top
				ShowFloat2("Left Top", animation2D_frame.left_top, 70.0f, 150.0f);

				//Frame Size
				ShowFloat2("Frame Size", animation2D_frame.frame_size, 70.0f, 150.0f);

				//Duration
				ShowFloat(m_p_sprite_animation->GetResourceName(), "Duration", animation2D_frame.duration, 150.0f);


				m_p_sprite_animation->SetAnimationFrame(m_frame_index, animation2D_frame);

				Vector2 uv_left_top = Vector2
				(
					animation2D_frame.left_top.x / m_atlas_texture_size.x,
					animation2D_frame.left_top.y / m_atlas_texture_size.y
				);
				Vector2 uv_right_bottom = Vector2
				(
					(animation2D_frame.left_top.x + animation2D_frame.frame_size.x) / m_atlas_texture_size.x,
					(animation2D_frame.left_top.y + animation2D_frame.frame_size.y) / m_atlas_texture_size.y
				);

				//Preview Animation
				ImVec2 preview_rect_size = ImVec2(animation2D_frame.frame_size.x * 2.0f, animation2D_frame.frame_size.y * 2.0f);

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
		m_draw_left_top = Vector2(animation2D_frame.left_top.x, animation2D_frame.left_top.y);
		m_draw_right_bottom = Vector2(m_draw_left_top.x + animation2D_frame.frame_size.x, m_draw_left_top.y + animation2D_frame.frame_size.y);

		//Canvas의 상대적인 좌표를 구함(= Atlas Texture 상에서의 left_top, right_bottom 좌표를 구함)
		m_draw_left_top.x += origin.x;
		m_draw_left_top.y += origin.y;

		m_draw_right_bottom.x += origin.x;
		m_draw_right_bottom.y += origin.y;

		//Canvas의 범위
		if (animation2D_frame.frame_size.x != 0.0f || animation2D_frame.frame_size.y != 0.0f)
		{
			m_draw_list->PushClipRect(origin, m_canvas_right_bottom, true);
			m_draw_list->AddRect(ImVec2(m_draw_left_top.x, m_draw_left_top.y), ImVec2(m_draw_right_bottom.x, m_draw_right_bottom.y), IM_COL32(0, 255, 0, 255));
			m_draw_list->PopClipRect();
		}
	}
}