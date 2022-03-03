#include "stdafx.h"
#include "GUI_Tree.h"

#include "GUI_TreeItem.h"

GUI_Tree::~GUI_Tree()
{
	Clear();
}

void GUI_Tree::Update()
{
	if (m_p_root_item == nullptr)
		return;

	if (m_is_visible_root)
		m_p_root_item->Update();

	else
	{
		for (auto& child_tree_item : m_p_root_item->m_p_child_vector)
			child_tree_item->Update();
	}
}

GUI_TreeItem* GUI_Tree::AddItem(GUI_TreeItem* p_parent, const std::string& item_name, const PayLoad& pay_load, const bool& use_drag_and_drop)
{
	auto p_tree_item = new GUI_TreeItem(p_parent, item_name, pay_load);
	p_tree_item->m_p_owner_tree = this;
	p_tree_item->m_use_drag_and_drop = use_drag_and_drop;

	//최상위 TreeItem인 경우
	if (p_parent == nullptr)
		m_p_root_item = p_tree_item;

	else
		p_parent->m_p_child_vector.emplace_back(p_tree_item);

	return p_tree_item;
}

void GUI_Tree::Clear()
{
	if (m_p_root_item == nullptr)
		return;

	std::queue<GUI_TreeItem*> p_tree_item_queue;
	p_tree_item_queue.push(m_p_root_item);

	//자식 Tree Item 너비 탐색
	while (!p_tree_item_queue.empty())
	{
		auto p_current_tree_item = p_tree_item_queue.front();
		p_tree_item_queue.pop();

		auto p_child_vector = p_current_tree_item->m_p_child_vector;
		for (auto& child : p_child_vector)
		{
			p_tree_item_queue.push(child);
		}

		SAFE_DELETE(p_current_tree_item);
	}

	m_p_root_item = nullptr;
	m_p_selected_item = nullptr;

	m_p_drag_start_item = nullptr;
	m_p_dropped_item = nullptr;
}

void GUI_Tree::ExcuteClickedEmptySpaceCallBack()
{
	m_p_drag_start_item = nullptr;
	m_p_selected_item = nullptr;

	if (m_p_clicked_empty_space != nullptr)
		m_p_clicked_empty_space();
}

void GUI_Tree::ExcuteClickedCallBack(GUI_TreeItem* p_item)
{
	m_p_drag_start_item = nullptr;
	m_p_selected_item = p_item;

	if (m_p_selected_item->m_pay_load.type == PayLoadType::GameObject)
	{
		if(m_p_clicked_func_2 != nullptr)
			m_p_clicked_func_2(std::get<DWORD_PTR>(m_p_selected_item->m_pay_load.data));
	}

	else
	{
		if (m_p_clicked_func_1 != nullptr)
			m_p_clicked_func_1(std::get<std::string>(m_p_selected_item->m_pay_load.data));
	}
}

void GUI_Tree::SetSelectedItem(const std::string& directory_path)
{
	if (m_p_root_item == nullptr)
		return;

	std::queue<GUI_TreeItem*> p_tree_item_queue;
	p_tree_item_queue.push(m_p_root_item);

	//자식 Tree Item 너비 탐색
	while (!p_tree_item_queue.empty())
	{
		auto p_current_tree_item = p_tree_item_queue.front();
		p_tree_item_queue.pop();

		if (std::get<std::string>(p_current_tree_item->m_pay_load.data)._Equal(directory_path))
		{
			m_p_selected_item = p_current_tree_item;
			return;
		}

		auto p_child_vector = p_current_tree_item->m_p_child_vector;
		for (auto& child : p_child_vector)
		{
			p_tree_item_queue.push(child);
		}
	}
}

void GUI_Tree::SetDroppedItem(GUI_TreeItem* p_item)
{
	m_p_dropped_item = p_item;

	if (m_p_drag_start_item != nullptr && m_p_drag_drop_func != nullptr)
		m_p_drag_drop_func((DWORD_PTR)m_p_dropped_item, (DWORD_PTR)m_p_drag_start_item);

	m_p_dropped_item = nullptr;
	m_p_drag_start_item = nullptr;
}
