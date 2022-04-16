#include "stdafx.h"
#include "PlayerSelect_Script.h"

#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>

PlayerSelect_Script::PlayerSelect_Script()
	:Script("PlayerSelect_Script")
{
}

PlayerSelect_Script::PlayerSelect_Script(const PlayerSelect_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

PlayerSelect_Script::~PlayerSelect_Script()
{
#pragma region X
	m_p_x_selected_focus = nullptr;
#pragma endregion

#pragma region Z
	m_p_z_selected_focus = nullptr;
	m_p_z_preview_animator = nullptr;
#pragma endregion
}

void PlayerSelect_Script::Awake()
{
#pragma region X
	auto p_child_game_object = m_p_owner_game_object->GetChildFromIndex(1);
	m_p_x_selected_focus = p_child_game_object->GetChildFromIndex(0);
#pragma endregion

#pragma region Z
	p_child_game_object = m_p_owner_game_object->GetChildFromIndex(2);
	m_p_z_selected_focus = p_child_game_object->GetChildFromIndex(0);

	m_p_z_preview_animator = p_child_game_object->GetChildFromIndex(2)->GetComponent<Animator2D>();
#pragma endregion

	PageController::m_p_audio_source = m_p_owner_game_object->GetComponent<AudioSource>();

	SetZFocus();

	AddAnimationEvent();
}

void PlayerSelect_Script::OnEnable()
{
	m_is_focus = true;
	SetZFocus();
}

void PlayerSelect_Script::Update()
{
	if (!m_is_focus)
		return;

#pragma region Input Arrow Left & Right
	//왼쪽 또는 오른쪽 방향키를 눌렀을 때
	if (KEY_DOWN(Key::KEY_ARROW_LEFT) || KEY_DOWN(Key::KEY_ARROW_RIGHT))
	{
		switch (m_p_current_selected)
		{
		case CurrentSelectedType::X:
			SetZFocus();
			break;
		case CurrentSelectedType::Z:
			SetXFocus();
			break;
		}

		SetCurrentAudioClip("PlayerSelect_Move_Sound", 1.0f);
	}
#pragma endregion

#pragma region Input Space
	//Space 키를 눌렀을 때
	if (KEY_DOWN(Key::KEY_SPACE))
	{
		switch (m_p_current_selected)
		{
		case CurrentSelectedType::X:
			SetXSeleted();
			break;
		case CurrentSelectedType::Z:
			SetZSeleted();
			break;
		}

		SetCurrentAudioClip("PlayerSelect_Select_Sound", 1.0f);
	}
#pragma endregion

#pragma region Input F1
	//F1 키를 눌렀을 때
	if (KEY_DOWN(Key::KEY_F1))
	{
		if (m_p_active_menu_page_func != nullptr)
			m_p_active_menu_page_func();

		m_p_owner_game_object->SetIsActive(false);
	}
#pragma endregion
}

void PlayerSelect_Script::SetXFocus()
{
	m_p_z_selected_focus->SetIsActive(false);
	SetCurrentAnimation("Z_Idle");

	m_p_x_selected_focus->SetIsActive(true);
	m_p_current_selected = CurrentSelectedType::X;
	//TODO
}

void PlayerSelect_Script::SetZFocus()
{
	m_p_x_selected_focus->SetIsActive(false);
	//TODO

	m_p_z_selected_focus->SetIsActive(true);
	m_p_current_selected = CurrentSelectedType::Z;
	SetCurrentAnimation("Z_Attack_1", 0.5f, true);
}

void PlayerSelect_Script::SetXSeleted()
{
	//TODO

	LOG_INFO_F("플레이어가 X를 선택했습니다.");
}

void PlayerSelect_Script::SetZSeleted()
{
	SetCurrentAnimation("Z_Win");
	m_is_focus = false;

	LOG_INFO_F("플레이어가 Z를 선택했습니다.");
}

void PlayerSelect_Script::AddAnimationEvent()
{
#pragma region X

#pragma endregion

#pragma region Z
	m_p_z_preview_animator->SetAnimationEvent("Z_Win", 32, std::bind(&PlayerSelect_Script::DisactiveSelf, this));
#pragma endregion

}

void PlayerSelect_Script::DisactiveSelf()
{
	if (m_p_active_loading_page_func != nullptr)
		m_p_active_loading_page_func();

	m_p_owner_game_object->SetIsActive(false);
}

void PlayerSelect_Script::SetCurrentAnimation(const std::string& animation_name, const float& animation_speed, const bool& is_loop, const bool& is_reverse)
{
	switch (m_p_current_selected)
	{
	case CurrentSelectedType::X:
		//TODO
		break;
	case CurrentSelectedType::Z:
	{
		if (m_p_z_preview_animator == nullptr)
			return;

		m_p_z_preview_animator->SetCurrentAnimation(animation_name);
		m_p_z_preview_animator->SetAnimationSpeed(animation_speed);
		m_p_z_preview_animator->SetIsLoop(is_loop);
		m_p_z_preview_animator->SetIsPlayReverse(is_reverse);
	}
	break;
	}
}

void PlayerSelect_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}