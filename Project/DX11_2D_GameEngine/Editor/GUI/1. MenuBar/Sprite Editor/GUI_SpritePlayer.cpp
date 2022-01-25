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
	curret_animation_state = static_cast<UINT>(AnimationState::Stop);
}

void GUI_SpritePlayer::Render()
{
	if (ImGui::Begin("Sprite Player", &m_is_active))
	{
		ShowSpriteAnimationPreview();
	
		ImGui::End();
	}
}

void GUI_SpritePlayer::ShowSpriteAnimationPreview()
{
	//Get Sprite Animation Data
	auto animation2D_frame = m_p_current_animation2D->GetAnimationFrame(m_frame_index);
	auto animation2D_data = animation2D_frame.animation2D_data;
	auto p_atlas_texture = m_p_current_animation2D->GetAtlasTexture();
	auto atlas_texture_size = Vector2(static_cast<float>(p_atlas_texture->GetWidth()), static_cast<float>(p_atlas_texture->GetHeight()));
	
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

	ImGui::Image
	(
		p_atlas_texture->GetShaderResourceView(),
		ImVec2(animation2D_data.full_frame_size.x, animation2D_data.full_frame_size.y),
		ImVec2(uv_left_top.x, uv_left_top.y),
		ImVec2(uv_right_bottom.x, uv_right_bottom.y),
		ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
		ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
	);

	//Frame Slider
	ImGui::SliderInt("##Sprite Animation Frame Slider", &m_frame_index, 0, m_p_current_animation2D->GetAnimationFrameCount() - 1);

	//재생 중일 때
	if ((curret_animation_state & AnimationState::Play) && !(curret_animation_state & AnimationState::Pause))
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

	ShowButtons();
}

void GUI_SpritePlayer::ShowButtons()
{
	//Play, Pause, Stop Button을 하나의 그룹으로 묶기
	ImGui::BeginGroup();

	auto icon_provider = IconProvider::GetInstance();

	//Animation Play Button 그리기
	//현재 애니메이션 상태가 Play라면 Play Button의 색상을 Active 색상으로 유지
	ImGui::PushStyleColor
	(
		ImGuiCol_Button,
		ImGui::GetStyle().Colors[curret_animation_state & AnimationState::Play ? ImGuiCol_ButtonActive : ImGuiCol_Button]
	);

	std::string unique_str = gui_str + "Play";
	ImGui::PushID(unique_str.c_str());
	if (icon_provider->CreateImageButton(IconType::ToolBar_Play, ImVec2(16.0f, 16.0f)))
	{
		if (curret_animation_state & AnimationState::Play)
			curret_animation_state = static_cast<UINT>(AnimationState::Stop);
		
		else
			curret_animation_state |= static_cast<UINT>(AnimationState::Play);
	}
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::PopStyleColor();

	//Animation Pause Button 그리기
	//현재 애니메이션 상태가 Play&Pause라면 Pause Button의 색상을 Active 색상으로 유지
	ImGui::PushStyleColor
	(
		ImGuiCol_Button,
		ImGui::GetStyle().Colors[curret_animation_state & AnimationState::Pause ? ImGuiCol_ButtonActive : ImGuiCol_Button]
	);

	unique_str = gui_str + "Pause";
	ImGui::PushID(unique_str.c_str());
	if (icon_provider->CreateImageButton(IconType::ToolBar_Pause, ImVec2(16.0f, 16.0f)))
	{
		curret_animation_state |= static_cast<UINT>(AnimationState::Pause);
	}
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::PopStyleColor();

	//Animation Pause Button 그리기
	unique_str = gui_str + "Stop";
	ImGui::PushID(unique_str.c_str());
	if (icon_provider->CreateImageButton(IconType::ToolBar_Stop, ImVec2(16.0f, 16.0f)))
	{
		curret_animation_state = static_cast<UINT>(AnimationState::Stop);
		m_frame_index = 0;
		m_accumulate_time = 0.0f;
	}
	ImGui::PopID();
	ImGui::EndGroup();
}
