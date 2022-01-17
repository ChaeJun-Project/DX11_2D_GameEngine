#pragma once
#include "GUI/IGUI.h"

//Tree
#include "GUI/Module/Tree/GUI_Tree.h"
#include "GUI/Module/Tree/GUI_TreeItem.h"

class Scene;
class GameObject;

class GUI_Hierarchy final : public IGUI
{
public:
	GUI_Hierarchy(const std::string& hierarchy_title);
	~GUI_Hierarchy();

public:
	void Initialize() override;
	void Update() override;
	void Render() override;

private:
    void UpdateTree();
	void AddGameObject(GUI_TreeItem* p_tree_item, GameObject* game_object);

	void CheckClickRightButton();
	void ShowHierarchy();
	
	void ClickEmptySpace();
    void ClickedGameObject(DWORD_PTR object_address);
    void DragDropGameObject(DWORD_PTR p_dropped_item, DWORD_PTR p_drag_start_item);

    void CheckEvnetKey();
	void DeleteGameObject(GameObject* game_object);
	
private:
	void ShowMenuPopup();
	void SelectedGameObject(GameObject* game_object);
	void CreateGameObject();

private:
	std::shared_ptr<Scene> m_p_current_scene = nullptr;

	GUI_Tree    m_gui_tree;

	Event_CallBack m_p_clicked_empty_space = nullptr;
	Clicked_CallBack2 m_p_clicked_func_2 = nullptr;
	DragDrop_CallBack m_p_drag_drop_func = nullptr;
};