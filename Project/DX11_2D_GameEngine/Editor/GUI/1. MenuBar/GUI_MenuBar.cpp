#include "stdafx.h"
#include "GUI_MenuBar.h"

#include "Scene Rename/GUI_SceneRename.h"
#include "Style Selector/GUI_StyleSelector.h"
#include "Sprite Editor/GUI_SpriteEditor.h"
#include "Physics/GUI_Physics.h"
#include "Set Resolution/GUI_SetResolution.h"

#include "Scene/ClientSceneManager.h"

#include "Manager/EditorManager.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>
#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>

GUI_MenuBar::GUI_MenuBar(const std::string& menubar_title)
	:IGUI(menubar_title)
{
	m_p_gui_scene_rename = std::make_unique<GUI_SceneRename>();
	m_p_gui_style_selector = std::make_unique<GUI_StyleSelector>();
	m_p_gui_sprite_editor = std::make_unique<GUI_SpriteEditor>();
	m_p_gui_physics = std::make_unique<GUI_Physics>();
	m_p_gui_set_resolution = std::make_unique<GUI_SetResolution>();
}

GUI_MenuBar::~GUI_MenuBar()
{
	m_p_gui_scene_rename.reset();
	m_p_gui_style_selector.reset();
	m_p_gui_sprite_editor.reset();
	m_p_gui_physics.reset();
	m_p_gui_set_resolution.reset();
}

void GUI_MenuBar::Initialize()
{
	InitializeRecentScene();
}

void GUI_MenuBar::Update()
{
	if (SCENE_MANAGER->GetEditorState() != EditorState::EditorState_Stop)
		return;

	if (KEY_PRESS(Key::KEY_CONTROL) && KEY_DOWN(Key::KEY_M))
	{
		m_is_active = !m_is_active;
	}

	if (m_is_active)
	{
		//=========================
		// File
		//=========================
		//New Scene
		if (KEY_PRESS(Key::KEY_CONTROL) && KEY_DOWN(Key::KEY_N))
		{
			NewScene();
		}

		//Load Scene
		if (KEY_PRESS(Key::KEY_CONTROL) && KEY_DOWN(Key::KEY_A))
		{
			LoadScene();
		}

		//Save Scene
		if (KEY_PRESS(Key::KEY_CONTROL) && KEY_DOWN(Key::KEY_S))
		{
			SaveScene();
		}

		//Rename Scene  
		if (KEY_PRESS(Key::KEY_CONTROL) && KEY_DOWN(Key::KEY_R))
		{
			m_p_gui_scene_rename->m_is_active = !m_p_gui_scene_rename->m_is_active;
		}

		//=========================
		// Edit
		//=========================
		//Style Editor
		if (KEY_PRESS(Key::KEY_CONTROL) && KEY_DOWN(Key::KEY_Y))
		{
			m_p_gui_style_selector->m_is_active = !m_p_gui_style_selector->m_is_active;
		}

		//Sprite Editor
		if (KEY_PRESS(Key::KEY_CONTROL) && KEY_DOWN(Key::KEY_D))
		{
			m_p_gui_sprite_editor->m_is_active = !m_p_gui_sprite_editor->m_is_active;
		}

		//Physics Editor
		if (KEY_PRESS(Key::KEY_CONTROL) && KEY_DOWN(Key::KEY_P))
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

			if (ImGui::MenuItem("Set Resolutuion"))
			{
				if (!m_p_gui_set_resolution->m_is_active)
				{
					m_p_gui_set_resolution->m_is_active = true;
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

	//Set Resolution Editor
	if (m_p_gui_set_resolution->m_is_active) m_p_gui_set_resolution->Render();
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
		EDITOR_MANAGER->ExcuteEventCallBack(); //�α� ����

		//Scene Change
		EventStruct event_struct;
		ZeroMemory(&event_struct, sizeof(EventStruct));

		event_struct.event_type = EventType::Scene_Change;
		event_struct.object_address_1 = new_scene;

		EVENT_MANAGER->AddEvent(event_struct);

		LOG_INFO_F("���ο� Scene�� ���������� ��������ϴ�.");
	}

	else
		SCENE_MANAGER->SetCurrentScene(current_scene);
}

void GUI_MenuBar::LoadScene()
{
	EDITOR_MANAGER->ExcuteEventCallBack(); //�α� ����
	auto load_scene_path = FileFunction::LoadFile(SCENE_PATH, FileType::Scene);
	FileFunction::LoadScene(load_scene_path);

	ClientSceneManager::m_recent_scene_name = FILE_MANAGER->GetOriginFileNameFromPath(load_scene_path);
	SaveRecentScene();
}

void GUI_MenuBar::SaveScene()
{
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	auto current_scene_path = SCENE_PATH;
	current_scene_path += (current_scene->GetSceneName() + ".scene");

	//���� Edit���� Scene�� Scene ������ �̹� ����Ǿ� �ִٸ�
	//Modal Window�� ������ʰ� ����
	if (FILE_MANAGER->IsExistFile(current_scene_path))
		ClientSceneManager::SaveScene(current_scene_path);

	else
		FileFunction::SaveFile(SCENE_PATH, current_scene->GetSceneName(), FileType::Scene);
}

void GUI_MenuBar::InitializeRecentScene()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_recent_scene_file_path);

	if (FILE_MANAGER->IsExistFile(path))
		LoadRecentScene();

	else
		SaveRecentScene();

	if (!ClientSceneManager::m_recent_scene_name.empty())
	{
		auto recent_scene_path = FILE_MANAGER->GetAbsoluteContentPath();
		recent_scene_path += ("Asset/Scene/" + ClientSceneManager::m_recent_scene_name + ".scene");

		FileFunction::LoadScene(recent_scene_path);
	}
}

void GUI_MenuBar::SaveRecentScene()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_recent_scene_file_path);

	FILE* p_file = nullptr;
	fopen_s(&p_file, path.c_str(), "wb");

	if (p_file != nullptr)
	{
		//Recent Scene
		fprintf(p_file, "[Recent Scene]\n");
		fprintf(p_file, "%s\n", ClientSceneManager::m_recent_scene_name.c_str());

		fclose(p_file);
	}
}

void GUI_MenuBar::LoadRecentScene()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_recent_scene_file_path);

	FILE* p_file = nullptr;

	fopen_s(&p_file, path.c_str(), "rb");
	if (p_file != nullptr)
	{
		char char_buffer[256] = { 0 };

		//Recent Scene
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Recent Scene]
		FILE_MANAGER->FScanf(char_buffer, p_file); //Recent Scene Name
		ClientSceneManager::m_recent_scene_name = std::string(char_buffer);

		fclose(p_file);
	}
}
