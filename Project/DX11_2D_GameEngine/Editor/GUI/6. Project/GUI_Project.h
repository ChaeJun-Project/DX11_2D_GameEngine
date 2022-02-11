#pragma once
#include "GUI/IGUI.h"

//Tree
#include "GUI/Module/Tree/GUI_Tree.h"
#include "GUI/Module/Tree/GUI_TreeItem.h"

class GUI_FileDialog;

class GUI_Project final : public IGUI
{
public:
	GUI_Project(const std::string& project_title);
	~GUI_Project();

public:
	void Initialize() override;
	void Update() override;
	void Render() override;

private:
	void UpdateTree();
	void AddDirectory(GUI_TreeItem* p_tree_item, const std::string& directory_path);

	void ShowUpdateButton();
	void ShowAssetHierarchy();

	void ShowFilter();
	void ShowFilesInDirectory();
	void ChangeDirectory(const std::string& current_directory);
	void DoubleClickedDirectory(const std::string& current_directory);
	
private:
	std::string m_root_name = "Asset";
	std::string m_root_path;
	std::string m_current_path;

	bool m_is_update = false;

	std::unique_ptr<GUI_Tree> m_directory_tree = nullptr;

	ImGuiTextFilter m_file_filter;
	Clicked_CallBack1 m_p_clicked_func_1 = nullptr;

	std::unique_ptr<GUI_FileDialog> m_p_file_dialog = nullptr;
};

