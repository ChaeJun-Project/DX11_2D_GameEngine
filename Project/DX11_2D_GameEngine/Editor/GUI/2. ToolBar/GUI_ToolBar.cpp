#include "stdafx.h"
#include "GUI_ToolBar.h"

//Helper
#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

GUI_ToolBar::GUI_ToolBar(const std::string& toolbar_title)
	:IGUI(toolbar_title)
{
	m_window_flags = 0
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;
}

void GUI_ToolBar::Update()
{
	if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_T))
	{
		m_is_active = !m_is_active;
	}
}

void GUI_ToolBar::Begin()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + 18.0f));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 15.0f));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(m_gui_title.c_str(), &m_is_active, m_window_flags);
	
}

void GUI_ToolBar::Render()
{
    ImGui::SameLine();

	//Play Button
	ImGui::PushStyleColor
	(
		ImGuiCol_Button,
		ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]
	);
	auto icon_provider = IconProvider::GetInstance();

	//Scene Play Button 그리기
	if (icon_provider->CreateImageButton("Scene Play", IconType::ToolBar_Play, ImVec2(22.0f, 22.0f)))
	{ 
		SceneManager::GetInstance()->SetEditorState(EditorState::EditorState_Play);
	    ImGui::SetWindowFocus(nullptr);
	}
	ImGui::SameLine();

	//Scene Pause Button 그리기
	if (icon_provider->CreateImageButton("Scene Pause", IconType::ToolBar_Pause, ImVec2(22.0f, 22.0f)))
	{ 
		SceneManager::GetInstance()->SetEditorState(EditorState::EditorState_Pause);
		ImGui::SetWindowFocus(nullptr);
	}
	ImGui::SameLine();

	//Scene Pause Button 그리기
	if (icon_provider->CreateImageButton("Scene Stop", IconType::ToolBar_Stop, ImVec2(22.0f, 22.0f)))
	{ 
		SceneManager::GetInstance()->SetEditorState(EditorState::EditorState_Stop);
		ImGui::SetWindowFocus(nullptr);
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}
