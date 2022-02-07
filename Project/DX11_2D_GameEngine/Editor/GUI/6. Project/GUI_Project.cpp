#include "stdafx.h"
#include "GUI_Project.h"

#include "File/GUI_FileDialog.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>
#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/EventManager.h>

GUI_Project::GUI_Project(const std::string& project_title)
	:IGUI(project_title)
{
	m_path_tree = std::make_unique<GUI_Tree>();

	m_p_clicked_func_1 = std::bind
	(
		&GUI_Project::ClickedDirectory,
		this,
		std::placeholders::_1
	);

	m_p_file_dialog = std::make_unique<GUI_FileDialog>();
}

GUI_Project::~GUI_Project()
{
	m_p_clicked_func_1 = nullptr;

	m_path_tree->Clear();
	m_path_tree.reset();
	m_p_file_dialog.reset();
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
		m_p_file_dialog->Update(m_current_path);
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
	m_path_tree->Clear();
	m_path_tree->SetIsVisibleRoot(false);

	//DX11_2D_GameEngine/ExeFile/Release/Content/
	std::string root_path = FILE_MANAGER->GetAbsoluteContentPath();
	root_path += m_root_name;

	PayLoad pay_load;
	pay_load.type = PayLoadType::Folder;
	pay_load.data = root_path; //DX11_2D_GameEngine/ExeFile/Release/Content/Asset

	auto p_root_tree_item = m_path_tree->AddItem(nullptr, m_root_name, pay_load, false);

	auto directory_path_vector = FILE_MANAGER->GetDirectoriesInDirectory(root_path);
	for (const auto& directory_path : directory_path_vector)
		AddDirectory(p_root_tree_item, directory_path);

	m_path_tree->SetClickedCallBack1(m_p_clicked_func_1);
}

void GUI_Project::AddDirectory(GUI_TreeItem* p_tree_item, const std::string& directory_path)
{
	if (p_tree_item == nullptr || directory_path.empty())
		return;

	auto tree_item_name = FILE_MANAGER->GetDirectoryFromPath(directory_path);

	PayLoad pay_load;
	pay_load.type = PayLoadType::Folder;
	pay_load.data = directory_path;

	auto p_current_tree_item = m_path_tree->AddItem(p_tree_item, tree_item_name, pay_load, false);
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
			m_path_tree->Update();
	}
	ImGui::EndChild();
}

void GUI_Project::ShowFilter()
{
	//Filter
	ImGui::PushItemWidth(300.0f);
	m_file_filter.Draw("##File Filter");
	ImGui::PopItemWidth();
}

void GUI_Project::ShowFilesInDirectory()
{
	ImGui::BeginGroup();
	//Current Path
	ImGui::PushItemWidth(300.0f);
	ImGui::InputText("##Current Path", &m_current_path, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopItemWidth();
	ImGui::Separator();

	ImGui::BeginChild("Files", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	{
		//Show Files
		m_p_file_dialog->Render(m_file_filter);
	}
	ImGui::EndChild();
	//Make GameObject -> Prefab
	//드랍 된 경우
	if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::GameObject))
	{
		
	}
	ImGui::EndGroup();

}

void GUI_Project::ClickedDirectory(const std::string& current_directory)
{
	if (!m_current_path.empty())
		m_current_path.clear();

	m_current_path = current_directory;

	m_p_file_dialog->Update(m_current_path);
}
