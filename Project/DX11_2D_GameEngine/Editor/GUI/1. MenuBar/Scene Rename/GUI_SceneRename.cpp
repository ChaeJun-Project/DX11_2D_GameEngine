#include "stdafx.h"
#include "GUI_SceneRename.h"

#include <DX11_2D_GameEngine_Lib/Scene.h>

void GUI_SceneRename::Render()
{
	if (ImGui::Begin("Rename Scene", &m_is_active, ImGuiWindowFlags_AlwaysAutoResize))
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

			SCENE_MANAGER->UpdateScene();

			LOG_INFO_F("현재 Scene의 이름이 [%s]으로 변경되었습니다.", scene_name.c_str());
		}
		ImGui::PopItemWidth();
	}
	ImGui::End();
}
