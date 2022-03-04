#include "stdafx.h"
#include "GUI_MenuBar.h"

#include "Scene Rename/GUI_SceneRename.h"
#include "Style Selector/GUI_StyleSelector.h"
#include "Sprite Editor/GUI_SpriteEditor.h"
#include "Physics/GUI_Physics.h"

#include "Scene/ClientSceneManager.h"

#include "Manager/EditorManager.h"

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

#include "Helper/EditorHelper.h"
void GUI_MenuBar::NewScene()
{
	EDITOR_HELPER->SetSelectedGameObject(nullptr);
	EDITOR_HELPER->SetSelectedResource(nullptr);

	//Current Scene
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	//New Scene
	auto new_scene = std::make_shared<Scene>("New Scene");
	new_scene->SetStartScene();
	SCENE_MANAGER->SetCurrentScene(new_scene);

	if (FileFunction::SaveFile(SCENE_PATH, "New Scene", FileType::Scene))
	{
		EDITOR_MANAGER->ExcuteEventCallBack(); //로그 삭제

		//Scene Change
		EventStruct event_struct;
		ZeroMemory(&event_struct, sizeof(EventStruct));

		event_struct.event_type = EventType::Scene_Change;
		event_struct.object_address_1 = new_scene;

		EVENT_MANAGER->AddEvent(event_struct);

		EDITOR_LOG_INFO_F("새로운 Scene을 성공적으로 만들었습니다.");
	}

	else
		SCENE_MANAGER->SetCurrentScene(current_scene);
}

void GUI_MenuBar::LoadScene()
{
	EDITOR_MANAGER->ExcuteEventCallBack(); //로그 삭제
	FileFunction::LoadScene(FileFunction::LoadFile(SCENE_PATH, FileType::Scene));
}

void GUI_MenuBar::SaveScene()
{
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	auto current_scene_path = SCENE_PATH;
	current_scene_path += (current_scene->GetSceneName() + ".scene");

	//현재 Edit중인 Scene이 Scene 폴더에 이미 저장되어 있다면
	//Modal Window를 띄우지않고 저장
	if (FILE_MANAGER->IsExistFile(current_scene_path))
		ClientSceneManager::SaveScene(current_scene_path);

	else
		FileFunction::SaveFile(SCENE_PATH, current_scene->GetSceneName(), FileType::Scene);
}
