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
	virtual void Initialize() override;
	virtual void Update() override;

	virtual void Render() override;

private:
    void UpdateTree();
	void AddGameObject(GUI_TreeItem* p_tree_item, GameObject* p_game_object);

	void CheckClickRightButton();
	void ShowHierarchy();
	
	void ClickEmptySpace();
    void ClickedGameObject(DWORD_PTR object_address);
    void DragDropGameObject(DWORD_PTR p_dropped_item, DWORD_PTR p_drag_start_item);

    void CheckEvnetKey();
	void DeleteGameObject(GameObject* p_game_object);
	
private:
	void ShowMenuPopup();
	void SelectedGameObject(GameObject* p_game_object);
	void CreateGameObject();

private:
	std::shared_ptr<Scene> m_p_current_scene = nullptr;

	std::unique_ptr<GUI_Tree> m_scene_hierarchy_tree = nullptr;

	Event_CallBack m_p_clicked_empty_space = nullptr;
	Clicked_CallBack2 m_p_clicked_func_2 = nullptr;
	DragDrop_CallBack m_p_drag_drop_func = nullptr;
};