#include "stdafx.h"
#include "GUI_MenuBar.h"

#include "Style Selector/GUI_StyleSelector.h"

#include "Sprite Editor/GUI_SpriteEditor.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>
#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>

GUI_MenuBar::GUI_MenuBar(const std::string& menubar_title)
	:IGUI(menubar_title)
{
	m_p_gui_style_selector = new GUI_StyleSelector;
	m_p_gui_sprite_editor = new GUI_SpriteEditor();
}

GUI_MenuBar::~GUI_MenuBar()
{
	SAFE_DELETE(m_p_gui_style_selector);
	SAFE_DELETE(m_p_gui_sprite_editor);
}

void GUI_MenuBar::Initialize()
{
}

void GUI_MenuBar::Update()
{
	if (SceneManager::GetInstance()->GetEditorState() != EditorState::EditorState_Stop)
		return;

	if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_M))
	{
		m_is_active = !m_is_active;
	}

	if (m_is_active)
	{
		//=========================
		// File
		//=========================
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_A))
		{
			LoadScene();
		}

		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_S))
		{
			SaveCurrentScene();
		}

		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_R))
		{
			m_is_show_rename_scene = !m_is_show_rename_scene;
		}

		//=========================
		// Edit
		//=========================
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_Y))
		{
			m_p_gui_style_selector->m_is_active = !m_p_gui_style_selector->m_is_active;
		}

		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_D))
		{
			m_p_gui_sprite_editor->m_is_active = !m_p_gui_sprite_editor->m_is_active;
		}
	}
}

void GUI_MenuBar::Render()
{
	if (SCENE_MANAGER->GetEditorState() != EditorState::EditorState_Stop)
		return;

	//Draw Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		//File
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Scene", "CTRL + A"))
			{
				LoadScene();
			}

			if (ImGui::MenuItem("Save Scene", "CTRL + S"))
			{
				SaveCurrentScene();
			}

			if (ImGui::MenuItem("Rename Current Scene", "CTRL + R"))
			{
				m_is_show_rename_scene = true;
			}

			ImGui::EndMenu();
		}

		//Edit
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Show Style Selector", "CTRL + Y"))
			{
				if (!m_p_gui_style_selector->m_is_active)
					m_p_gui_style_selector->m_is_active = true;
			}

			if (ImGui::MenuItem("Sprite Animation", "CTRL + D"))
			{
				if (!m_p_gui_sprite_editor->m_is_active)
				{
					m_p_gui_sprite_editor->Initialize();
					m_p_gui_sprite_editor->m_is_active = true;
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//Rename Scene
	if (m_is_show_rename_scene) ShowRenameScene();

	//Style Editor
	if (m_p_gui_style_selector->m_is_active) m_p_gui_style_selector->Render();

	//Sprite Editor
	if (m_p_gui_sprite_editor->m_is_active) m_p_gui_sprite_editor->Render();
}

void GUI_MenuBar::ShowRenameScene()
{
	if (ImGui::Begin("Rename Scene", &m_is_show_rename_scene))
	{
		auto current_scene = SCENE_MANAGER->GetCurrentScene();
		std::string scene_name = current_scene->GetSceneName();

		//Scene Name
		ImGui::Text("Scene Name");
		ImGui::SameLine();

		ImGui::PushItemWidth(200.0f);
		if (ImGui::InputText("##Scene Name", &scene_name, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			FILE_MANAGER->RenameFile(SCENE_PATH, ".scene", current_scene->GetSceneName(), scene_name);
			current_scene->SetSceneName(scene_name);
			EDITOR_LOG_INFO_F("Success to Rename Current Scene '%s'", scene_name.c_str());
		}
		ImGui::PopItemWidth();

		ImGui::End();
	}
}

void GUI_MenuBar::SaveCurrentScene()
{
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	FileFunction::SaveFile(SCENE_PATH, current_scene->GetSceneName(), FileType::Scene);
}

void GUI_MenuBar::LoadScene()
{
	FileFunction::LoadScene(FileFunction::LoadFile(SCENE_PATH, FileType::Scene));
}
