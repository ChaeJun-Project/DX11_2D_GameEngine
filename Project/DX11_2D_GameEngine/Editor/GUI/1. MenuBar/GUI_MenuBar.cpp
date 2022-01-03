#include "stdafx.h"
#include "GUI_MenuBar.h"

#include "Style Selector/GUI_StyleSelector.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>

GUI_MenuBar::GUI_MenuBar(const std::string& menubar_title)
	:IGUI(menubar_title)
{
	m_p_gui_style_selector = new GUI_StyleSelector;
}

GUI_MenuBar::~GUI_MenuBar()
{
	SAFE_DELETE(m_p_gui_style_selector);
}

void GUI_MenuBar::Initialize()
{
}

void GUI_MenuBar::Update()
{
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
			LoadFile("Scene/", FileType::Scene);
		}

		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_S))
		{
			SaveFile("Scene/", FileType::Scene);
		}

		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_R))
		{
			m_is_show_rename_scene = !m_is_show_rename_scene;
		}

		//=========================
		// Edit
		//=========================
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_D))
		{
			m_is_show_demo = !m_is_show_demo;
		}

		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_Y))
		{
			m_is_show_style = !m_is_show_style;
		}

	}
}

void GUI_MenuBar::Render()
{
	//Draw Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Scene", "CTRL + A"))
			{
				LoadFile("Scene/", FileType::Scene);
			}

			if (ImGui::MenuItem("Save Scene", "CTRL + S"))
			{
				SaveFile("Scene/", FileType::Scene);
			}

			if (ImGui::MenuItem("Rename Current Scene", "CTRL + R"))
			{
				m_is_show_rename_scene = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Show Demo", "CTRL + D", &m_is_show_demo))
			{
				
			}

			if (ImGui::MenuItem("Show Style Selector", "CTRL + Y", &m_is_show_style))
			{
				m_p_gui_style_selector->m_is_active = m_is_show_style;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if(m_is_show_rename_scene) ShowRenameScene();

	if(m_is_show_demo) ImGui::ShowDemoWindow(&m_is_show_demo);

	if (!m_p_gui_style_selector->m_is_active)
	{
		m_is_show_style = false;
	}

	if(m_is_show_style) m_p_gui_style_selector->Render();
}

void GUI_MenuBar::ShowRenameScene()
{
	if (ImGui::Begin("Rename Scene", &m_is_show_rename_scene))
	{
	    auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
		std::string scene_name = current_scene->GetSceneName();

		//Scene Name
		ImGui::Text("Scene Name");
		ImGui::SameLine();

		ImGui::PushItemWidth(200.0f);
		if (ImGui::InputText("##Scene Name", &scene_name, 1000))
		{
			FileManager::RenameFileName("Scene/", ".scene", current_scene->GetSceneName(), scene_name);
			current_scene->SetSceneName(scene_name);
			EDITOR_LOG_INFO_F("Success to Rename Current Scene '%s'", scene_name.c_str());
		}
		ImGui::PopItemWidth();

		ImGui::End();
	}
}
