#include "stdafx.h"
#include "GUI_Project.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>
#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/EventManager.h>

GUI_Project::GUI_Project(const std::string& project_title)
	:IGUI(project_title)
{
	//DX11_2D_GameEngine/ExeFile/Release/Content/
	m_root_path = FILE_MANAGER->GetAbsoluteContentPath();
	m_root_path += m_root_name;

	m_p_clicked_func_1 = std::bind
	(
		&GUI_Project::ClickedDirectory,
		this,
		std::placeholders::_1
	);
}

void GUI_Project::Initialize()
{
	UpdateTree();
}

void GUI_Project::Update()
{
	if (SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop)
	{
		if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_P))
		{
			m_is_active = !m_is_active;
		}
	}

	//Asset 폴더 포함 하위 폴더에 변경이 있는 경우 
	if (m_is_update)
	{
		UpdateTree();
		m_is_update = false;
	}
}

void GUI_Project::Render()
{
	ImGui::BeginGroup();
	ShowUpdateButton();
	ImGui::SameLine();
	ShowFilter();
	ImGui::Separator();

	ShowAssetHierarchy();
	ImGui::SameLine();

	ShowFilesInDirectory();
	ImGui::EndGroup();
}

void GUI_Project::UpdateTree()
{
	m_gui_tree.Clear();
	m_gui_tree.SetIsVisibleRoot(false);

	PayLoad pay_load;
	pay_load.type = PayLoadType::Folder;
	pay_load.data = m_root_path;

	auto p_root_tree_item = m_gui_tree.AddItem(nullptr, m_root_name, pay_load);

	auto directory_path_vector = FILE_MANAGER->GetDirectoriesInDirectory(m_root_path);
	for (const auto& directory_path : directory_path_vector)
		AddDirectory(p_root_tree_item, directory_path);

	m_gui_tree.SetClickedCallBack1(m_p_clicked_func_1);
}

void GUI_Project::AddDirectory(GUI_TreeItem* p_tree_item, const std::string& directory_path)
{
	if (p_tree_item == nullptr || directory_path.empty())
		return;

	auto tree_item_name = FILE_MANAGER->GetDirectoryFromPath(directory_path);

	PayLoad pay_load;
	pay_load.type = PayLoadType::Folder;
	pay_load.data = directory_path;

	auto p_current_tree_item = m_gui_tree.AddItem(p_tree_item, tree_item_name, pay_load);
	auto directory_path_vector = FILE_MANAGER->GetDirectoriesInDirectory(directory_path);

	for (const auto& directory_path : directory_path_vector)
		AddDirectory(p_current_tree_item, directory_path);
}

void GUI_Project::ShowUpdateButton()
{
	if (ImGui::Button("Update Directories", ImVec2(150.0f, 0.0f)))
	{
		if (SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop)
			m_is_update = true;
	}
}

void GUI_Project::ShowAssetHierarchy()
{
	ImGui::BeginChild("Folders", ImVec2(150, 0), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	{
		if (ImGui::CollapsingHeader("Asset", ImGuiTreeNodeFlags_DefaultOpen))
			m_gui_tree.Update();
	}
	ImGui::EndChild();
}

void GUI_Project::ShowFilter()
{
	//Filter
	m_file_filter.Draw("##File Filter");
}

void GUI_Project::ShowFilesInDirectory()
{
	ImGui::BeginGroup();
	//Current Path
	ImGui::InputText("##Current Path", &m_current_path, ImGuiInputTextFlags_ReadOnly);
	ImGui::Separator();

	ImGui::BeginChild("Files", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	{
		//Show Files

	}
	ImGui::EndChild();
	ImGui::EndGroup();
}

void GUI_Project::ClickedDirectory(const std::string& current_directory)
{
	if (!m_current_path.empty())
		m_current_path.clear();

	m_current_path = current_directory;
}
