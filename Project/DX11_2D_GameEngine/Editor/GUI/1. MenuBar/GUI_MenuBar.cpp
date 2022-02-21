#include "stdafx.h"
#include "GUI_MenuBar.h"

#include "Scene Rename/GUI_SceneRename.h"
#include "Style Selector/GUI_StyleSelector.h"
#include "Sprite Editor/GUI_SpriteEditor.h"
#include "Physics/GUI_Physics.h"

#include "Scene/ClientSceneManager.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>
#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>

GUI_MenuBar::GUI_MenuBar(const std::string& menubar_title)
	:IGUI(menubar_title)
{
	m_p_gui_scene_rename = new GUI_SceneRename();
	m_p_gui_style_selector = new GUI_StyleSelector();
	m_p_gui_sprite_editor = new GUI_SpriteEditor();
	m_p_gui_physics = new GUI_Physics();
}

GUI_MenuBar::~GUI_MenuBar()
{
	SAFE_DELETE(m_p_gui_scene_rename);
	SAFE_DELETE(m_p_gui_style_selector);
	SAFE_DELETE(m_p_gui_sprite_editor);
	SAFE_DELETE(m_p_gui_physics);
}

void GUI_MenuBar::Update()
{
	if (SCENE_MANAGER->GetEditorState() != EditorState::EditorState_Stop)
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
		//New Scene
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_N))
		{
			NewScene();
		}

		//Load Scene
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_A))
		{
			LoadScene();
		}

		//Save Scene
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_S))
		{
			SaveScene();
		}

		//Rename Scene  
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_R))
		{
			m_p_gui_scene_rename->m_is_active = !m_p_gui_scene_rename->m_is_active;
		}

		//=========================
		// Edit
		//=========================
		//Style Editor
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_Y))
		{
			m_p_gui_style_selector->m_is_active = !m_p_gui_style_selector->m_is_active;
		}

		//Sprite Editor
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_D))
		{
			m_p_gui_sprite_editor->m_is_active = !m_p_gui_sprite_editor->m_is_active;
		}

		//Physics Editor
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_P))
		{
			m_p_gui_physics->m_is_active = !m_p_gui_physics->m_is_active;
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
			if (ImGui::MenuItem("New Scene", "CTRL + N"))
			{
				NewScene();
			}

			if (ImGui::MenuItem("Load Scene", "CTRL + A"))
			{
				LoadScene();
			}

			if (ImGui::MenuItem("Save Scene", "CTRL + S"))
			{
				SaveScene();
			}

			if (ImGui::MenuItem("Rename Scene", "CTRL + R"))
			{
				if (!m_p_gui_scene_rename->m_is_active)
					m_p_gui_scene_rename->m_is_active = true;
			}

			ImGui::EndMenu();
		}

		//Edit
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Style Selector", "CTRL + Y"))
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

			if (ImGui::MenuItem("Physics", "CTRL + P"))
			{
				if (!m_p_gui_physics->m_is_active)
				{	
					m_p_gui_physics->m_is_active = true;
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//Scene Rename
	if (m_p_gui_scene_rename->m_is_active) m_p_gui_scene_rename->Render();

	//Style Editor
	if (m_p_gui_style_selector->m_is_active) m_p_gui_style_selector->Render();

	//Sprite Editor
	if (m_p_gui_sprite_editor->m_is_active) m_p_gui_sprite_editor->Render();

	//Physics Editor
	if (m_p_gui_physics->m_is_active) m_p_gui_physics->Render();
}

void GUI_MenuBar::NewScene()
{
	ClientSceneManager::CreateNewScene();
}

void GUI_MenuBar::LoadScene()
{
	FileFunction::LoadScene(FileFunction::LoadFile(SCENE_PATH, FileType::Scene));
}

void GUI_MenuBar::SaveScene()
{
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	FileFunction::SaveFile(SCENE_PATH, current_scene->GetSceneName(), FileType::Scene);
}
