#include "stdafx.h"
#include "GUI_SpriteEditor.h"

#include "GUI/GUI_ItemList.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>

#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/Animation2D.h>

GUI_SpriteEditor::GUI_SpriteEditor()
{
	m_p_atlas_item_list = new GUI_ItemList;
}

GUI_SpriteEditor::~GUI_SpriteEditor()
{
	//Atlas Texture
	m_p_atlas_texture.reset();
	SAFE_DELETE(m_p_atlas_item_list);

	//Animator2D
	m_p_current_animater = nullptr;

	//Animation2D
	m_p_animation2D.reset();
	m_p_animation_item_list = nullptr;

	draw_list = nullptr;
}

void GUI_SpriteEditor::Render()
{
	if (ImGui::Begin("Sprite Editor", &m_is_active, ImGuiWindowFlags_NoMove))
	{
		ImGui::BulletText("Atlas Texture");

		//Show Altas Texture Combo
		ImGui::PushItemWidth(150.0f);
		{
			auto resource_manager = ResourceManager::GetInstance();

			int index = 0;

			const auto& atlas_texture_map = resource_manager->GetAtlasTextureMap();

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
						m_p_atlas_texture = resource_manager->GetAtlasTexture(item_list_vector[i]);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			m_p_atlas_item_list->ClearItemList();

			ImGui::PopItemWidth();
		}

		if (m_p_atlas_texture != nullptr)
		{
			atlas_texture_size = Vector2::Zero;
			atlas_texture_size = Vector2(static_cast<float>(m_p_atlas_texture->GetWidth()), static_cast<float>(m_p_atlas_texture->GetHeight()));

			std::string atlas_texture_name = m_p_atlas_texture->GetResourceName() + "(" + std::to_string(m_p_atlas_texture->GetWidth()) + " X " + std::to_string(m_p_atlas_texture->GetHeight()) + ")";
			ImGui::Text(atlas_texture_name.c_str());

			ImGui::Separator();

			//Canvas(Atlas Texture) Range
			draw_list = ImGui::GetWindowDrawList();

			canvas_left_top = ImGui::GetCursorScreenPos();
			canvas_right_bottom = canvas_left_top + ImVec2(atlas_texture_size.x, atlas_texture_size.y);
			ImGui::Image
			(
				m_p_atlas_texture->GetShaderResourceView(),
				ImVec2(atlas_texture_size.x, atlas_texture_size.y),
				ImVec2(0.0f, 0.0f),
				ImVec2(1.0f, 1.0f),
				ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
				ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
			);

			const bool is_hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held

			ImGui::SameLine();

			if (m_p_atlas_texture != nullptr)


				ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
			ImGui::SameLine();

			RenderAnimationEditor();

			DrawRect(is_hovered, is_active);
		}

		ImGui::End();
	}
}

void GUI_SpriteEditor::RenderAnimationEditor()
{
	if (m_p_current_animater == nullptr || m_p_atlas_texture == nullptr)
		return;

	ImGui::BeginGroup();
	static std::string animation_name;

	ImGui::Text("Animation Name");
	ImGui::SameLine();
	ImGui::PushItemWidth(150.0f);
	ImGui::InputText("##", &animation_name);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	//Create Animation Button
	if (ImGui::Button("New Animation", ImVec2(140.0f, 0.0f)))
	{
		if (animation_name.size())
		{
			m_p_animation2D = m_p_current_animater->CreateAnimation(animation_name, m_p_atlas_texture); //Game Object의 이름을 수정한 경우에만 수행
			animation_name = "";
		}
	}

	//성공적으로 애니메이션이 만들어졌다면
	if (m_p_animation2D != nullptr)
	{
		//Show Altas Texture Combo
		ImGui::PushItemWidth(150.0f);
		{
			int index = 0;

			std::string selected_animation_name = m_p_animation2D->GetAnimationName();

			const auto& item_list_vector = m_p_animation_item_list->GetItemList();

			for (const auto& animation : item_list_vector)
			{
				if (animation == selected_animation_name)
					m_p_animation_item_list->SetCurrentListID(index);

				else
					++index;
			}

			if (ImGui::BeginCombo("##Animation2D List", selected_animation_name.c_str()))
			{
				for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
				{
					const bool is_selected = (*(m_p_animation_item_list->GetCurrentListID()) == i);
					if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
					{
						m_p_animation2D = m_p_current_animater->GetAnimation(item_list_vector[i]);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			m_p_animation_item_list->ClearItemList();

			ImGui::PopItemWidth();
		}

		ImGui::Text("Animation Frame ID");

		Animation2D_Frame animation2D_frame;
		ZeroMemory(&animation2D_frame, sizeof(Animation2D_Frame));

		if (m_p_animation2D->HasAnimationFrame())
		{
			animation2D_frame = m_p_animation2D->GetAnimationFrame(static_cast<UINT>(frame_index));

			total_frame_count = m_p_animation2D->GetAnimationFrameCount() - 1;

			//Frame Index(Frame Data가 1개라도 존재한느 경우에만 렌더링)
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);
			static int slider_index = 0;
			if (ImGui::SliderInt("", &slider_index, 0, total_frame_count))
			{
				frame_index = slider_index;
				animation2D_frame = m_p_animation2D->GetAnimationFrame(static_cast<UINT>(frame_index));
			}
			ImGui::PopItemWidth();

			//Left Top
			ShowFloat2("Left Top", animation2D_frame.animation2D_data.left_top, 150.0f);

			//Frame Size
			ShowFloat2("Frame Size", animation2D_frame.animation2D_data.frame_size, 150.0f);

			//Full Frame Size
			ShowFloat2("Full Frame Size", animation2D_frame.animation2D_data.full_frame_size, 150.0f);

			//Offset
			ShowFloat2("Offset", animation2D_frame.animation2D_data.offset, 150.0f);

			//Duration
			ShowFloat(m_p_animation2D->GetAnimationName(), "Duration", animation2D_frame.duration, 150.0f);


			m_p_animation2D->SetAnimationFrame(frame_index, animation2D_frame);

			Animation2D_Data animation2D_data = animation2D_frame.animation2D_data;

			Vector2 uv_left_top = Vector2
			(
				animation2D_data.left_top.x / atlas_texture_size.x + animation2D_data.offset.x / animation2D_data.full_frame_size.x,
				animation2D_data.left_top.y / atlas_texture_size.y + animation2D_data.offset.y / animation2D_data.full_frame_size.y
			);
			Vector2 uv_right_bottom = Vector2
			(
				(animation2D_data.left_top.x + animation2D_data.full_frame_size.x) / atlas_texture_size.x + animation2D_data.offset.x / animation2D_data.full_frame_size.x,
				(animation2D_data.left_top.y + animation2D_data.full_frame_size.y) / atlas_texture_size.y + animation2D_data.offset.y / animation2D_data.full_frame_size.y
			);

			//Preview Animation
			ImVec2 preview_rect_size = ImVec2(animation2D_frame.animation2D_data.full_frame_size.x * 2.0f, animation2D_frame.animation2D_data.full_frame_size.y * 2.0f);
			ImVec2 frame_rect_size = ImVec2(animation2D_frame.animation2D_data.frame_size.x * 2.0f, animation2D_frame.animation2D_data.frame_size.y * 2.0f);

			ImGui::BulletText("Preview Frame");

			ImVec2 current_screen_pos = ImGui::GetCursorScreenPos();

			ImGui::Image
			(
				m_p_atlas_texture->GetShaderResourceView(),
				preview_rect_size,
				ImVec2(uv_left_top.x, uv_left_top.y),
				ImVec2(uv_right_bottom.x, uv_right_bottom.y),
				ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
				ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
			);

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

			//Canvas의 범위
			draw_list->PushClipRect(current_screen_pos, current_screen_pos + preview_rect_size, true);
			//Vertical Cross
			draw_list->AddLine
			(
				ImVec2(current_screen_pos.x + (preview_rect_size.x / 2), current_screen_pos.y + 1.0f),
				ImVec2(current_screen_pos.x + (preview_rect_size.x / 2), current_screen_pos.y + preview_rect_size.y + 1.0f),
				IM_COL32(255, 0, 0, 200),
				3.0f
			);

			//Horizontal Cross
			draw_list->AddLine
			(
				ImVec2(current_screen_pos.x + 1.0f, current_screen_pos.y + (preview_rect_size.y / 2)),
				ImVec2(current_screen_pos.x + preview_rect_size.x + 1.0f, current_screen_pos.y + (preview_rect_size.y / 2)),
				IM_COL32(0, 0, 255, 200),
				3.0f
			);
			
			//Frame Rect
			draw_list->AddRect(ImVec2(preview_frame_rect_left_top.x, preview_frame_rect_left_top.y), ImVec2(preview_frame_rect_right_bottom.x, preview_frame_rect_right_bottom.y), IM_COL32(0, 255, 0, 255));
			draw_list->PopClipRect();
		}

		if (ImGui::Button("Create Frame", ImVec2(120.0f, 0.0f)))
		{
			ZeroMemory(&animation2D_frame, sizeof(Animation2D_Frame));
			m_p_animation2D->CreateAnimationFrame(animation2D_frame);
			start_point_in_canvas = ImVec2(0.0f, 0.0f);
			end_point_in_canvas = ImVec2(0.0f, 0.0f);
		}

		if (ImGui::Button("Delete Frame", ImVec2(120.0f, 0.0f)))
		{
			frame_index = m_p_animation2D->DeleteAnimationFrame(frame_index);
		}
	}

	else
	{

	}
	ImGui::EndGroup();

}

void GUI_SpriteEditor::DrawRect(const bool& is_hovered, const bool& is_active)
{
	if (m_p_animation2D == nullptr)
		return;

	if (m_p_animation2D->HasAnimationFrame())
	{
		//Canvas의 시작점
		const ImVec2 origin(canvas_left_top.x, canvas_left_top.y);

		Animation2D_Frame animation2D_frame = m_p_animation2D->GetAnimationFrame(static_cast<UINT>(frame_index));

		//Draw Rect
		draw_left_top = Vector2(animation2D_frame.animation2D_data.left_top.x, animation2D_frame.animation2D_data.left_top.y);
		draw_right_bottom = Vector2(draw_left_top.x + animation2D_frame.animation2D_data.full_frame_size.x, draw_left_top.y + animation2D_frame.animation2D_data.full_frame_size.y);

		//Canvas의 상대적인 좌표를 구함(= Atlas Texture 상에서의 left_top, right_bottom 좌표를 구함)
		draw_left_top.x += origin.x;
		draw_left_top.y += origin.y;

		draw_right_bottom.x += origin.x;
		draw_right_bottom.y += origin.y;

		//Canvas의 범위
		if (animation2D_frame.animation2D_data.full_frame_size.x != 0.0f || animation2D_frame.animation2D_data.full_frame_size.y != 0.0f)
		{
			draw_list->PushClipRect(origin, canvas_right_bottom, true);
			draw_list->AddRect(ImVec2(draw_left_top.x, draw_left_top.y), ImVec2(draw_right_bottom.x, draw_right_bottom.y), IM_COL32(0, 255, 0, 255));
			draw_list->PopClipRect();
		}
	}
}

const bool GUI_SpriteEditor::CheckMousePositionInRect(const ImVec2& mouse_position, const Vector2& rect_left_top, const Vector2& rect_right_bottom)
{
	//현재 마우스 커서의 위치가 Rect 내부에 있는 경우
	if (rect_left_top.x < mouse_position.x && rect_left_top.y < mouse_position.y &&
		rect_right_bottom.x > mouse_position.x && rect_right_bottom.y > mouse_position.y)
		return true;

	else
		return false;
}

void GUI_SpriteEditor::Store()
{
	//if (m_p_animation2D == nullptr)
	//	return;

	//static bool is_new_draw = false;
	//static bool is_rect_move = false;
	//static bool is_adding_rect = false;

	//// Draw border and background color
	//ImGuiIO& io = ImGui::GetIO();

	////Canvas의 시작점
	//const ImVec2 origin(canvas_left_top.x, canvas_left_top.y);
	//const ImVec2 mouse_pos_in_canvas = ImVec2(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	////현재 마우스 위치가 그려진 사각형 내부에 위치해있지 않다면
	//if (!CheckMousePositionInRect(mouse_pos_in_canvas, draw_left_top, draw_right_bottom))
	//{
	//	//Add start and end point
	//	if (is_hovered && !is_adding_rect && ImGui::IsMouseClicked(0))
	//	{
	//		start_point_in_canvas = mouse_pos_in_canvas;
	//		m_mouse_pos_in_canvas = mouse_pos_in_canvas;
	//		is_adding_rect = true;
	//	}
	//	if (is_adding_rect)
	//	{
	//		end_point_in_canvas = mouse_pos_in_canvas;
	//		m_mouse_pos_in_canvas = mouse_pos_in_canvas;
	//		if (!ImGui::IsMouseDown(0))
	//			is_adding_rect = false;

	//		is_new_draw = true;
	//	}
	//}

	//else
	//{
	//	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	//	{
	//		start_point_in_canvas.x += io.MouseDelta.x;
	//		start_point_in_canvas.y += io.MouseDelta.y;

	//		end_point_in_canvas.x += io.MouseDelta.x;
	//		end_point_in_canvas.y += io.MouseDelta.y;

	//		is_rect_move = true;
	//	}
	//}


	////Draw Rect
	//draw_left_top = Vector2(origin.x + start_point_in_canvas.x, origin.y + start_point_in_canvas.y);
	//draw_right_bottom = Vector2(origin.x + end_point_in_canvas.x, origin.y + end_point_in_canvas.y);

	//if (m_p_animation2D->HasAnimationFrame())
	//{
	//	Animation2D_Frame animation2D_frame = m_p_animation2D->GetAnimationFrame(static_cast<UINT>(frame_index));

	//	//Canvas의 범위
	//	draw_list->PushClipRect(origin, canvas_right_bottom, true);
	//	draw_list->AddRect(ImVec2(draw_left_top.x, draw_left_top.y), ImVec2(draw_right_bottom.x, draw_right_bottom.y), IM_COL32(0, 255, 0, 255));
	//	draw_list->PopClipRect();

	//	//Canvas의 상대적인 좌표를 구함(= Atlas Texture 상에서의 left_top, right_bottom 좌표를 구함)
	//	draw_left_top.x -= origin.x;
	//	draw_left_top.y -= origin.y;

	//	draw_right_bottom.x -= origin.x;
	//	draw_right_bottom.y -= origin.y;

	//	Vector2 draw_full_frame_size = (draw_left_top - draw_right_bottom);
	//	draw_full_frame_size.x = Math::Abs(draw_full_frame_size.x);
	//	draw_full_frame_size.y = Math::Abs(draw_full_frame_size.y);

	//	if (is_new_draw || is_rect_move)
	//	{
	//		animation2D_frame.animation2D_data.left_top = draw_left_top;
	//		animation2D_frame.animation2D_data.full_frame_size = draw_full_frame_size;
	//		m_p_animation2D->SetAnimationFrame(frame_index, animation2D_frame);

	//		if (is_new_draw)
	//			is_new_draw = false;

	//		if (is_rect_move)
	//			is_rect_move = false;
	//	}
	//}
}
