#include "stdafx.h"
#include "GUI_Project.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

#include <DX11_2D_GameEngine_Lib/EventManager.h>

GUI_Project::GUI_Project(const std::string& project_title)
	:IGUI(project_title)
{
	//DX11_2D_GameEngine/ExeFile/Release/Content/
	m_root_path = FileManager::GetAbsoluteContentPath();
	m_root_path += m_root_name;

	m_p_clicked_func = std::bind
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
	if (EventManager::GetInstance()->IsUpdate())
	{
		UpdateTree();
	}
}

void GUI_Project::Render()
{
	ShowAssetHierarchy();
}

void GUI_Project::UpdateTree()
{
	m_gui_tree.Clear();
	m_gui_tree.SetIsVisibleRoot(false);

	auto p_root_tree_item = m_gui_tree.AddItem(nullptr, m_root_name, PayLoadType::Folder, 0);

	auto directory_path_vector = FileManager::GetDirectoriesInDirectory(m_root_path);
	for (const auto& directory_path : directory_path_vector)
		AddDirectory(p_root_tree_item, directory_path);

	m_gui_tree.SetClickedCallBack(m_p_clicked_func);
}

void GUI_Project::AddDirectory(GUI_TreeItem* p_tree_item, const std::string& directory_path)
{
	if (p_tree_item == nullptr || directory_path.empty())
		return;

	auto tree_item_name = FileManager::GetDirectoryFromPath(directory_path);

	auto p_current_tree_item = m_gui_tree.AddItem(p_tree_item, tree_item_name, PayLoadType::Folder, (DWORD_PTR)(&directory_path));
	auto directory_path_vector = FileManager::GetDirectoriesInDirectory(directory_path);

	for (const auto& directory_path : directory_path_vector)
		AddDirectory(p_current_tree_item, directory_path);
}

void GUI_Project::ShowAssetHierarchy()
{
	if (ImGui::CollapsingHeader(m_root_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		m_gui_tree.Update();
}

void GUI_Project::ShowFilesInDirectory()
{
}

void GUI_Project::ClickedDirectory(DWORD_PTR object_address)
{

}
