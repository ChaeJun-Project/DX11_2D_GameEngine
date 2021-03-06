#pragma once

class GUI_Tree;
class GameObject;

class GUI_TreeItem final
{
public:
	GUI_TreeItem() = default;
	GUI_TreeItem
	(
		GUI_TreeItem* p_parent,
		const std::string& item_name,
		const PayLoad& pay_load
	);
	~GUI_TreeItem();

public:
	void Update();

private:
    void CheckOpenTreeNode();
    void DragAndDrop();
	const bool CheckClickEmptySpace();
	const bool CheckClickMouseLeftButton();

public:
    const std::string& GetItemName() { return m_item_name; }
    void SetItemName(const std::string& item_name) { m_item_name = item_name; }

	const DWORD_PTR& GetPayLoadData() { return std::get<DWORD_PTR>(m_pay_load.data); }

private:
	GUI_Tree* m_p_owner_tree = nullptr;

	UINT m_guid = 0;
	std::string m_item_name;
	GUI_TreeItem* m_p_parent = nullptr;
	PayLoad m_pay_load;
	GameObject* p_game_object = nullptr;

	std::vector<GUI_TreeItem*> m_p_child_vector;

	bool m_use_drag_and_drop = false;
	bool m_clicked_empty_space = false;
	bool m_press_mouse_left_button = false;

	friend class GUI_Tree;
};

