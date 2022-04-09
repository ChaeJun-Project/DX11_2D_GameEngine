#include "stdafx.h"
#include "Menu_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/ImageRenderer.h>

Menu_Script::Menu_Script()
	:Script("Menu_Script")
{
	//Continue
	m_continue_button_texture_path = FILE_MANAGER->GetAbsoluteTexturePath() + m_continue_button_texture_path;

	//Game Start
	m_game_start_button_texture_path = FILE_MANAGER->GetAbsoluteTexturePath() + m_game_start_button_texture_path;
}

Menu_Script::Menu_Script(const Menu_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

Menu_Script::~Menu_Script()
{
#pragma region Button Object ImageRenderer
	m_p_continue_image_renderer = nullptr;
	m_p_game_start_image_renderer = nullptr;
#pragma endregion

#pragma region Button Image
	//Continue
	for (auto& p_texture : m_p_continue_button_textrue_vector)
	{
		p_texture.reset();
	}
	m_p_continue_button_textrue_vector.clear();
	m_p_continue_button_textrue_vector.shrink_to_fit();

	//Player Select
	for (auto& p_texture : m_p_game_start_button_textrue_vector)
	{
		p_texture.reset();
	}
	m_p_game_start_button_textrue_vector.clear();
	m_p_game_start_button_textrue_vector.shrink_to_fit();
#pragma endregion
}

void Menu_Script::Awake()
{
#pragma region Continue Button
	auto p_child_game_object = m_p_owner_game_object->GetChildFromIndex(0);
	m_p_continue_image_renderer = p_child_game_object->GetComponent<ImageRenderer>();
	LoadContinueButtonTexture();
#pragma endregion

#pragma region Player Select Button
	p_child_game_object = m_p_owner_game_object->GetChildFromIndex(1);
	m_p_game_start_image_renderer = p_child_game_object->GetComponent<ImageRenderer>();
	LoadPlayerSelectButtonTexture();
#pragma endregion

	PageController::m_p_audio_source = m_p_owner_game_object->GetComponent<AudioSource>();

	m_is_focus = true;

	SetGameStartButtonSelected();
}

void Menu_Script::OnEnable()
{
	m_is_focus = true;
	SetGameStartButtonSelected();
}

void Menu_Script::Update()
{
	if (!m_is_focus)
		return;

#pragma region Input Arrow Up & Down
	//위 또는 아래 방향키를 눌렀을 때
	if (KEY_DOWN(Key::KEY_ARROW_UP) || KEY_DOWN(Key::KEY_ARROW_DOWN))
	{
		switch (m_p_current_selected)
		{
		case CurrentSelectedType::Continue:
			SetGameStartButtonSelected();
			break;
		case CurrentSelectedType::GameStart:
			SetContinueButtonSelected();
			break;
		}

		SetCurrentAudioClip("Menu_Move_Sound", 1.0f);
	}
#pragma endregion

#pragma region Input Enter
	//Enter 키를 눌렀을 때
	if (KEY_DOWN(Key::KEY_ENTER))
	{
		switch (m_p_current_selected)
		{
		case CurrentSelectedType::Continue:
			if (m_p_active_loading_page_func != nullptr)
				m_p_active_loading_page_func();
			break;
		case CurrentSelectedType::GameStart:
			if (m_p_active_player_select_page_func != nullptr)
				m_p_active_player_select_page_func();
			break;
		}

		m_is_focus = false;
		m_p_owner_game_object->SetIsActive(false);
		SetCurrentAudioClip("Menu_Select_Sound", 1.0f);
	}
#pragma endregion
}

void Menu_Script::LoadContinueButtonTexture()
{
	auto texture_directory_path_vector = FILE_MANAGER->GetFilesInDirectory(m_continue_button_texture_path);
	for (const auto& texture_path : texture_directory_path_vector)
	{
		m_p_continue_button_textrue_vector.emplace_back(RESOURCE_MANAGER->LoadFromFile<Texture>(texture_path));
	}
}

void Menu_Script::LoadPlayerSelectButtonTexture()
{
	auto texture_directory_path_vector = FILE_MANAGER->GetFilesInDirectory(m_game_start_button_texture_path);
	for (const auto& texture_path : texture_directory_path_vector)
	{
		m_p_game_start_button_textrue_vector.emplace_back(RESOURCE_MANAGER->LoadFromFile<Texture>(texture_path));
	}
}

void Menu_Script::SetContinueButtonSelected()
{
	m_p_game_start_image_renderer->SetTexture(m_p_game_start_button_textrue_vector[0]);

	m_p_continue_image_renderer->SetTexture(m_p_continue_button_textrue_vector[1]);
	m_p_current_selected = CurrentSelectedType::Continue;
}

void Menu_Script::SetGameStartButtonSelected()
{
	m_p_continue_image_renderer->SetTexture(m_p_continue_button_textrue_vector[0]);

	m_p_game_start_image_renderer->SetTexture(m_p_game_start_button_textrue_vector[1]);
	m_p_current_selected = CurrentSelectedType::GameStart;
}

void Menu_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}