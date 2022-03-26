#include "stdafx.h"
#include "GUI_Project.h"

#include "Scene/ClientSceneManager.h"

#include "File/GUI_FileDialog.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/EventManager.h>

GUI_Project::GUI_Project(const std::string& project_title)
	:IGUI(project_title)
{
	//DX11_2D_GameEngine/ExeFile/Release/Content/
	m_root_path = FILE_MANAGER->GetAbsoluteContentPath();
	m_root_path += m_root_name;

	m_directory_tree = std::make_unique<GUI_Tree>();

	m_p_clicked_func_1 = std::bind
	(
		&GUI_Project::ChangeDirectory,
		this,
		std::placeholders::_1
	);

	m_p_file_dialog = std::make_unique<GUI_FileDialog>();
	m_p_file_dialog->m_p_update_file_dialog_func = std::bind
	(
		&GUI_Project::UpdateFileDialog,
		this,
		std::placeholders::_1
	); 
	
	//초기 경로를 Root Path로 설정
	ChangeDirectory(m_root_path);
}

GUI_Project::~GUI_Project()
{
	m_p_clicked_func_1 = nullptr;

	m_directory_tree->Clear();
	m_directory_tree.reset();
	m_p_file_dialog.reset();
}

void GUI_Project::Initialize()
{
	UpdateTree();
}

void GUI_Project::Update()
{
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
	m_directory_tree->Clear();
	m_directory_tree->SetIsVisibleRoot(true);

	PayLoad pay_load;
	pay_load.type = PayLoadType::Folder;
	pay_load.data = m_root_path; //DX11_2D_GameEngine/ExeFile/Release/Content/Asset

	auto p_root_tree_item = m_directory_tree->AddItem(nullptr, m_root_name, pay_load, false);

	auto directory_path_vector = FILE_MANAGER->GetDirectoriesInDirectory(m_root_path);
	for (const auto& directory_path : directory_path_vector)
		AddDirectory(p_root_tree_item, directory_path);

	m_directory_tree->SetClickedCallBack1(m_p_clicked_func_1);
}

void GUI_Project::AddDirectory(GUI_TreeItem* p_tree_item, const std::string& directory_path)
{
	if (p_tree_item == nullptr || directory_path.empty())
		return;

	auto tree_item_name = FILE_MANAGER->GetDirectoryFromPath(directory_path);

	PayLoad pay_load;
	pay_load.type = PayLoadType::Folder;
	pay_load.data = directory_path;

	auto p_current_tree_item = m_directory_tree->AddItem(p_tree_item, tree_item_name, pay_load, false);
	auto directory_path_vector = FILE_MANAGER->GetDirectoriesInDirectory(directory_path);

	for (const auto& directory_path : directory_path_vector)
		AddDirectory(p_current_tree_item, directory_path);
}

void GUI_Project::ShowUpdateButton()
{
	if (ImGui::Button("Update Directories", ImVec2(150.0f, 0.0f)))
	{
		if (SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Stop)
			m_is_update = true;
	}
}

void GUI_Project::ShowAssetHierarchy()
{
	ImGui::BeginChild("Folders", ImVec2(150, 0), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	{
		if (ImGui::CollapsingHeader(m_root_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			m_directory_tree->Update();
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
	std::string relative_current_path = FILE_MANAGER->GetRelativeResourcePathFromAbsolutePath(m_current_path);
	ImGui::InputText("##Current Path", &relative_current_path, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopItemWidth();
	ImGui::Separator();

	ImGui::BeginChild("Files", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	{
		//Show Files
		m_p_file_dialog->Render(m_file_filter);
	}
	ImGui::EndChild();

	//드랍 된 경우
	//Make GameObject -> Prefab
	if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::GameObject))
	{
		FileFunction::CreatePrefabResource(std::get<DWORD_PTR>(pay_load->data));
	}
	ImGui::EndGroup();
}

void GUI_Project::ChangeDirectory(const std::string& current_directory)
{
	m_current_path = current_directory;

	m_p_file_dialog->Update(m_current_path);
}

void GUI_Project::UpdateFileDialog(const std::string& current_directory)
{
	m_directory_tree->SetSelectedItem(current_directory);

	ChangeDirectory(current_directory);
}
