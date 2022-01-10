#pragma once
#include "GUI/IGUI.h"

//Tree
#include "GUI/Module/Tree/GUI_Tree.h"
#include "GUI/Module/Tree/GUI_TreeItem.h"

class GUI_Project final : public IGUI
{
public:
    GUI_Project(const std::string& project_title);
    ~GUI_Project() = default;

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
    void ClickedDirectory(const std::string& current_directory);

private:
    std::string m_root_name = "Asset";
    std::string m_root_path;

    std::string m_current_path;

    bool m_is_update = false;

    GUI_Tree    m_gui_tree;

    ImGuiTextFilter m_file_filter;
    Clicked_CallBack1 m_p_clicked_func_1 = nullptr;
};

