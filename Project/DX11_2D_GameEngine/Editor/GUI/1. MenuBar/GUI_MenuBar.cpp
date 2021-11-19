#include "stdafx.h"
#include "GUI_MenuBar.h"

GUI_MenuBar::GUI_MenuBar(const std::string& menubar_title)
	:IGUI(menubar_title)
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
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_S))
		{
			m_is_scene_save = true;
		}

		//=========================
		// Edit
		//=========================
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_D))
		{
			m_is_show_demo = !m_is_show_demo;
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
			if (ImGui::MenuItem("Load Scene", "CTRL + A", &m_is_scene_load))
			{

			}

			if (ImGui::MenuItem("Save Scene", "CTRL + S", &m_is_scene_save))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Show Demo", "CTRL + D", &m_is_show_demo))
			{
				
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if(m_is_show_demo) ImGui::ShowDemoWindow(&m_is_show_demo);
}
