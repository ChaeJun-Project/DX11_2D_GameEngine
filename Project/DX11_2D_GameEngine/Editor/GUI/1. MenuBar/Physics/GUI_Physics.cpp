#include "stdafx.h"
#include "GUI_Physics.h"

#include <DX11_2D_GameEngine_Lib/CollisionManager.h>

GUI_Physics::GUI_Physics()
{
	InitializePhysics();
}

GUI_Physics::~GUI_Physics()
{
	m_collision_check_map.clear();
}

void GUI_Physics::Render()
{
	if (ImGui::Begin("Physics", &m_is_active, ImGuiWindowFlags_AlwaysAutoResize))
		ShowLayerCollisionMatrix();

	ImGui::End();
}

void GUI_Physics::ShowLayerCollisionMatrix()
{
	int column_count = MAX_LAYER + 1; //Matrix 열의 개수

	ImGui::BulletText("Layer Collision Matrix");
	if (ImGui::BeginTable("Layer Collision Matrix", column_count))
	{
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
		for (int i = (MAX_LAYER - 1); i >= 0; --i)
		{
			ImGui::Text(std::to_string(i).c_str());

			if (i != 0)
				ImGui::TableNextColumn();
		}

		for (int row = 0; row < MAX_LAYER; ++row)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(std::to_string(row).c_str());
			ImGui::TableNextColumn();
			for (int column = (MAX_LAYER - 1); column >= row; --column) //column 인덱스 = row 인덱스와 충돌 체크를 할 대상 Layer의 Index
			{
				bool is_collision_check = m_collision_check_map[row][column - row];
				std::string label_str = "##" + std::to_string(row) + std::to_string(column);
				if (ImGui::Checkbox(label_str.c_str(), &is_collision_check))
				{
					is_collision_check != is_collision_check;
					m_collision_check_map[row][column - row] = is_collision_check;
					COLLISION_MANAGER->CheckLayer(row, column);
					SavePhysics();
				}
				ImGui::TableNextColumn();
			}
		}
		ImGui::EndTable();
	}
}

void GUI_Physics::InitializePhysics()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_physics_file_path);

	if (FILE_MANAGER->IsExistFile(path))
		LoadPhysics();

	else
	{
		SavePhysics();
		LoadPhysics();
	}
}

void GUI_Physics::SavePhysics()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_physics_file_path);

	FILE* p_file = nullptr;
	fopen_s(&p_file, path.c_str(), "wb");

	if (p_file != nullptr)
	{
		const auto& collision_check_vector = COLLISION_MANAGER->GetCollisionCheckVector();

		fprintf(p_file, "[Collision Check List]\n");
		for (UINT i = 0; i < static_cast<UINT>(collision_check_vector.size()); ++i)
		{
			fprintf(p_file, "%d %u\n", i, collision_check_vector[i]);
		}

		fclose(p_file);
	}
}

void GUI_Physics::LoadPhysics()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_physics_file_path);

	FILE* p_file = nullptr;

	fopen_s(&p_file, path.c_str(), "rb");
	if (p_file != nullptr)
	{
		char char_buffer[256] = { 0 };

		//Collision Check List
		FILE_MANAGER->FScanf(char_buffer, p_file);
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			//Index
			UINT index = 0;
			fscanf_s(p_file, "%d", &index);

			//Add Collision Check Vector
			std::vector<bool> collision_check_vector;
			collision_check_vector.resize((MAX_LAYER - index), false);
			m_collision_check_map.insert(std::make_pair(index, collision_check_vector));

			//Collision Check Value
			UINT collision_check_value = 0;
			fscanf_s(p_file, "%d", &collision_check_value);

			for (UINT j = i; j < MAX_LAYER; ++j)
			{
				if (collision_check_value & (1 << j))
				{
					m_collision_check_map[index][j - index] = true;
					COLLISION_MANAGER->CheckLayer(index, j);
				}
			}
		}

		fclose(p_file);
	}
}
