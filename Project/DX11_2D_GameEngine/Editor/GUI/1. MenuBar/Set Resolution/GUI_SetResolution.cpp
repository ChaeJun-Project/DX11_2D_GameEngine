#include "stdafx.h"
#include "GUI_SetResolution.h"

GUI_SetResolution::GUI_SetResolution()
{
	InitializeResolution();
}

void GUI_SetResolution::Render()
{
	if (ImGui::Begin("Set Resolutuon", &m_is_active, ImGuiWindowFlags_AlwaysAutoResize))
	{	//Set Resolutuon
	    ShowFloat2("Set Resolution", m_resolution, 100.0f, 120.0f);
		ImGui::SameLine();

		if (ImGui::Button("Edit", ImVec2(80.0f, 0.0f)))
		{
			SETTINGS->SetGameResolution(m_resolution);
			SaveResolution();
		}
	}
	ImGui::End();
}

void GUI_SetResolution::InitializeResolution()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_resolution_file_path);

	if (FILE_MANAGER->IsExistFile(path))
		LoadResolution();

	else
	{
		SaveResolution();
		LoadResolution();
	}
}

void GUI_SetResolution::SaveResolution()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_resolution_file_path);

	FILE* p_file = nullptr;
	fopen_s(&p_file, path.c_str(), "wb");

	if (p_file != nullptr)
	{
		//Resolution
		fprintf(p_file, "[Resolution]\n");
		FILE_MANAGER->FPrintf_Vector2(m_resolution, p_file);
		
		fclose(p_file);
	}
}

void GUI_SetResolution::LoadResolution()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_resolution_file_path);

	FILE* p_file = nullptr;

	fopen_s(&p_file, path.c_str(), "rb");
	if (p_file != nullptr)
	{
		char char_buffer[256] = { 0 };

		//Resolution
		FILE_MANAGER->FScanf(char_buffer, p_file);
		FILE_MANAGER->FScanf_Vector2(m_resolution, p_file);

		SETTINGS->SetGameResolution(m_resolution);

		fclose(p_file);
	}
}
