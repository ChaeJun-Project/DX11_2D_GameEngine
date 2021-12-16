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

GUI_TreeItem* GUI_Tree::AddItem(GUI_TreeItem* p_parent, const std::string& item_name, const PayLoadType& pay_load_type, DWORD_PTR object_address)
{
	auto p_tree_item = new GUI_TreeItem(p_parent, item_name, pay_load_type, object_address);
	p_tree_item->m_p_owner_tree = this;

	//�ֻ��� TreeItem�� ���
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

	//�ڽ� Tree Item �ʺ� Ž��
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

void GUI_Tree::ExcuteClickedCallBack(GUI_TreeItem* p_item)
{
	m_p_selected_item = p_item;

	if (m_p_clicked_func != nullptr)
		m_p_clicked_func(std::get<DWORD_PTR>(m_p_selected_item->m_pay_load.data));
}

void GUI_Tree::SetDroppedItem(GUI_TreeItem* p_item)
{
	if (p_item == nullptr)
		return;

	m_p_dropped_item = p_item;

	if (m_p_drag_start_item != nullptr && m_p_drag_drop_func != nullptr)
		m_p_drag_drop_func((DWORD_PTR)m_p_dropped_item, (DWORD_PTR)m_p_drag_start_item);

	m_p_dropped_item = nullptr;
	m_p_drag_start_item = nullptr;
}