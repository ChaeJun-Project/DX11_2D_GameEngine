#include "stdafx.h"
#include "GUI_SpritePlayer.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

GUI_SpritePlayer::GUI_SpritePlayer()
{
}

GUI_SpritePlayer::~GUI_SpritePlayer()
{
	m_p_current_animation2D.reset();
	gui_str.clear();
}

void GUI_SpritePlayer::Initialize()
{
	m_p_current_animation2D.reset();
	m_frame_index = 0;
	m_accumulate_time = 0.0f;

	//Animation State
	m_curret_animation_state = static_cast<UINT>(AnimationState::Stop);
}

void GUI_SpritePlayer::Render()
{
	if (ImGui::Begin("Sprite Player", &m_is_active, ImGuiWindowFlags_AlwaysAutoResize))
		ShowSpriteAnimationPreview();

	ImGui::End();
}

void GUI_SpritePlayer::ShowSpriteAnimationPreview()
{
	//Get Sprite Animation Data
	auto animation2D_frame = m_p_current_animation2D->GetAnimationFrame(m_frame_index);
	auto p_atlas_texture = m_p_current_animation2D->GetAtlasTexture();
	auto atlas_texture_size = Vector2(static_cast<float>(p_atlas_texture->GetWidth()), static_cast<float>(p_atlas_texture->GetHeight()));

	auto begin_cursor_screen_pos = ImGui::GetCursorScreenPos();
	auto end_cursor_screen_pos = begin_cursor_screen_pos;
	end_cursor_screen_pos.x += 2.0f * animation2D_frame.frame_size.x;
	end_cursor_screen_pos.y += animation2D_frame.frame_size.y;

	Vector2 uv_left_top = Vector2
	(
		animation2D_frame.left_top.x / atlas_texture_size.x,
		animation2D_frame.left_top.y / atlas_texture_size.y
	);
	Vector2 uv_right_bottom = Vector2
	(
		(animation2D_frame.left_top.x + animation2D_frame.frame_size.x) / atlas_texture_size.x,
		(animation2D_frame.left_top.y + animation2D_frame.frame_size.y) / atlas_texture_size.y
	);

	auto offset = animation2D_frame.frame_size.x * 0.5f;
	ImGui::SetCursorScreenPos(ImVec2(begin_cursor_screen_pos.x + offset, begin_cursor_screen_pos.y));

	ImGui::Image
	(
		p_atlas_texture->GetShaderResourceView(),
		ImVec2(animation2D_frame.frame_size.x, animation2D_frame.frame_size.y),
		ImVec2(uv_left_top.x, uv_left_top.y),
		ImVec2(uv_right_bottom.x, uv_right_bottom.y)
	);

	//Draw Back Rect
	auto draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRect(begin_cursor_screen_pos, end_cursor_screen_pos, IM_COL32(255, 255, 255, 255));

	//Frame Slider
	ImGui::PushItemWidth(ImGui::GetItemRectSize().x * 2.0f);
	ImGui::SliderInt("##Sprite Animation Frame Slider", &m_frame_index, 0, m_p_current_animation2D->GetAnimationFrameCount() - 1);
	ImGui::PopItemWidth();

	//재생 중일 때
	if ((m_curret_animation_state & AnimationState::Play) && !(m_curret_animation_state & AnimationState::Pause))
	{
		auto delta_time_f = DELTA_TIME_F;
		m_accumulate_time += delta_time_f;

		if (animation2D_frame.duration <= m_accumulate_time)
		{
			m_accumulate_time = 0.0f;

			++m_frame_index;
			if (m_p_current_animation2D->GetAnimationFrameCount() == m_frame_index)
			{
				m_frame_index = 0;
			}
		}
	}

	ShowButtons(ImGui::GetItemRectSize().x);
}

void GUI_SpritePlayer::ShowButtons(const float& rect_size_width)
{
	auto offset_x = (rect_size_width - 88.0f) * 0.5f; //88.0은 세 가지 버튼 그룹의 총 고정 길이 

	auto begin_cursor_screen_pos = ImGui::GetCursorScreenPos();
	ImGui::SetCursorScreenPos(ImVec2(begin_cursor_screen_pos.x + offset_x, begin_cursor_screen_pos.y));

	//Play, Pause, Stop Button을 하나의 그룹으로 묶기
	ImGui::BeginGroup();

	auto icon_provider = IconProvider::GetInstance();

	//Animation Play Button 그리기
	//현재 애니메이션 상태가 Play라면 Play Button의 색상을 Active 색상으로 유지
	ImGui::PushStyleColor
	(
		ImGuiCol_Button,
		ImGui::GetStyle().Colors[m_curret_animation_state & AnimationState::Play ? ImGuiCol_ButtonActive : ImGuiCol_Button]
	);

	std::string unique_str = gui_str + "Play";
	ImGui::PushID(unique_str.c_str());
	if (icon_provider->CreateImageButton(IconType::ToolBar_Play, ImVec2(16.0f, 16.0f)))
	{
		if (m_curret_animation_state & AnimationState::Play)
		{
			m_frame_index = 0;
			m_accumulate_time = 0.0f;
			m_curret_animation_state = static_cast<UINT>(AnimationState::Stop);
		}

		else
			m_curret_animation_state |= static_cast<UINT>(AnimationState::Play);
	}
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::PopStyleColor();

	//Animation Pause Button 그리기
	//현재 애니메이션 상태가 Play&Pause라면 Pause Button의 색상을 Active 색상으로 유지
	ImGui::PushStyleColor
	(
		ImGuiCol_Button,
		ImGui::GetStyle().Colors[m_curret_animation_state & AnimationState::Pause ? ImGuiCol_ButtonActive : ImGuiCol_Button]
	);

	unique_str = gui_str + "Pause";
	ImGui::PushID(unique_str.c_str());
	if (icon_provider->CreateImageButton(IconType::ToolBar_Pause, ImVec2(16.0f, 16.0f)))
	{
		if (m_curret_animation_state & AnimationState::Pause)
			m_curret_animation_state &= ~static_cast<UINT>(AnimationState::Pause);

		else
			m_curret_animation_state |= static_cast<UINT>(AnimationState::Pause);
	}
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::PopStyleColor();

	//Animation Pause Button 그리기
	unique_str = gui_str + "Stop";
	ImGui::PushID(unique_str.c_str());
	if (icon_provider->CreateImageButton(IconType::ToolBar_Stop, ImVec2(16.0f, 16.0f)))
	{
		m_frame_index = 0;
		m_accumulate_time = 0.0f;
		m_curret_animation_state = static_cast<UINT>(AnimationState::Stop);
	}
	ImGui::PopID();
	ImGui::EndGroup();
}
