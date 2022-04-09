#include "stdafx.h"
#include "GUI_ToolBar.h"

//Client Scene
#include "Scene/ClientSceneManager.h"

//Helper
#include "Helper/IconProvider.h"
#include "Manager/EditorManager.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

GUI_ToolBar::GUI_ToolBar(const std::string& toolbar_title)
	:IGUI(toolbar_title)
{
	m_window_flags = ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoSavedSettings;
}

void GUI_ToolBar::Begin()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + 18.0f));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 0.0f));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 2.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);

	__super::Begin();
}

void GUI_ToolBar::Render()
{
	ImGui::SameLine();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 126.0f) * 0.5f);

	//Play, Pause, Stop Button을 하나의 그룹으로 묶기
	ImGui::BeginGroup();

	//현재 Editor 상태가 Play라면 Play Button의 색상을 Active 색상으로 유지
	ImGui::PushStyleColor
	(
		ImGuiCol_Button,
		ImGui::GetStyle().Colors[SCENE_MANAGER->GetEditorState() & EditorState::EditorState_Play ? ImGuiCol_ButtonActive : ImGuiCol_Button]
	);

	//Scene Play Button 그리기
	if (ICON_PROVIDER->CreateImageButton(IconType::ToolBar_Play, ImVec2(22.0f, 22.0f)))
	{
		EDITOR_MANAGER->ExcuteEventCallBack(); //로그 삭제
		if (SCENE_MANAGER->GetEditorState() & EditorState::EditorState_Play) //Play 중인데 다시 Play 버튼을 누른 경우 => Play -> Stop
			ClientSceneManager::InitializeScene(); //Scene 초기화(다시 로드)

		SCENE_MANAGER->SetEditorState(EditorState::EditorState_Play);
		ImGui::SetWindowFocus(nullptr);
	}
	ImGui::SameLine();

	ImGui::PopStyleColor();

	//현재 Editor 상태가 Play&Pause라면 Pause Button의 색상을 Active 색상으로 유지
	ImGui::PushStyleColor
	(
		ImGuiCol_Button,
		ImGui::GetStyle().Colors[SCENE_MANAGER->GetEditorState() & EditorState::EditorState_Pause ? ImGuiCol_ButtonActive : ImGuiCol_Button]
	);

	//Scene Pause Button 그리기
	if (ICON_PROVIDER->CreateImageButton(IconType::ToolBar_Pause, ImVec2(22.0f, 22.0f)))
	{
		if (SCENE_MANAGER->GetEditorState() & EditorState_Play)
		{
			SCENE_MANAGER->SetEditorState(EditorState::EditorState_Pause);
			ImGui::SetWindowFocus(nullptr);
		}
	}
	ImGui::SameLine();

	ImGui::PopStyleColor();

	//Scene Stop Button 그리기
	if (ICON_PROVIDER->CreateImageButton(IconType::ToolBar_Stop, ImVec2(22.0f, 22.0f)))
	{
		EDITOR_MANAGER->ExcuteEventCallBack(); //로그 삭제
		ClientSceneManager::InitializeScene(); //Scene 초기화(다시 로드)

		SCENE_MANAGER->SetEditorState(EditorState::EditorState_Stop);
		ImGui::SetWindowFocus(nullptr);
	}
	ImGui::EndGroup();

	ImGui::PopStyleColor();
}

void GUI_ToolBar::End()
{
	__super::End();
	ImGui::PopStyleVar();
}
