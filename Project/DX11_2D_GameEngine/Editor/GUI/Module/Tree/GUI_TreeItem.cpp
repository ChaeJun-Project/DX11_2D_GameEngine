#include "stdafx.h"
#include "GUI_TreeItem.h"

#include "GUI_Tree.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>

GUI_TreeItem::GUI_TreeItem(GUI_TreeItem* p_parent, const std::string& item_name, const PayLoad& pay_load)
	: m_p_parent(p_parent),
	m_item_name(item_name)
{
	m_pay_load = pay_load;

	if (m_pay_load.type == PayLoadType::GameObject)
	{
		p_game_object = (GameObject*)(std::get<DWORD_PTR>(m_pay_load.data));
	}
}

GUI_TreeItem::~GUI_TreeItem()
{
	p_game_object = nullptr;

	m_p_child_vector.clear();
	m_p_child_vector.shrink_to_fit();
}

void GUI_TreeItem::Update()
{
	if (p_game_object != nullptr)
	{
		m_item_name = p_game_object->GetGameObjectName();
	}

	bool clicked_empty_space = false;
	bool press_mouse_left_button = false;

	//TreeItem이 자식을 가지고 있는 경우 더블 클릭 or 왼쪽 화살표를 눌렀을 경우 펼침
	int flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

	//TreeItem이 자식을 가지고 있지 않은 경우
	if (m_p_child_vector.empty())
		flags |= ImGuiTreeNodeFlags_Leaf;  //해당 TreeItem는 잎으로 그림

	//현재 선택된 아이탬이 자신 또는 현재 드래그 시작 아이탬이 자신일 경우
	if (m_p_owner_tree->GetSelectedItem() == this || m_p_owner_tree->GetDragStartItem() == this)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (m_pay_load.type == PayLoadType::GameObject)
	{
		auto object = (DX11Obejct*)(std::get<DWORD_PTR>(m_pay_load.data));

		if (ImGui::TreeNodeEx((const void*)(object->GetObjectID()), flags, m_item_name.c_str()))
		{
			if (m_use_drag_and_drop)
				DragAndDrop();

			clicked_empty_space = CheckClickEmptySpace();
			press_mouse_left_button = CheckClickMouseLeftButton();

			for (auto& child_tree_item : m_p_child_vector)
				child_tree_item->Update();

			//노드 추가를 중지함
			ImGui::TreePop();
		}

		else
		{
			if (m_use_drag_and_drop)
				DragAndDrop();

			clicked_empty_space = CheckClickEmptySpace();
			press_mouse_left_button = CheckClickMouseLeftButton();
		}
	}

	else
	{
		if (ImGui::TreeNodeEx(m_item_name.c_str(), flags, m_item_name.c_str()))
		{
			if (m_use_drag_and_drop)
				DragAndDrop();

			clicked_empty_space = CheckClickEmptySpace();
			press_mouse_left_button = CheckClickMouseLeftButton();

			for (auto& child_tree_item : m_p_child_vector)
				child_tree_item->Update();

			//노드 추가를 중지함
			ImGui::TreePop();
		}

		else
		{
			if (m_use_drag_and_drop)
				DragAndDrop();

			clicked_empty_space = CheckClickEmptySpace();
			press_mouse_left_button = CheckClickMouseLeftButton();
		}
	}

	if (clicked_empty_space)
	{
		m_p_owner_tree->ExcuteClickedEmptySpaceCallBack();
		m_clicked_empty_space = false;
	}

	if (press_mouse_left_button)
	{
		m_p_owner_tree->ExcuteClickedCallBack(this);
		m_press_mouse_left_button = false;
	}
}

void GUI_TreeItem::DragAndDrop()
{
	//드래그 시작
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		DragDropEvent::CreateDragDropPayLoad(m_pay_load);
		ImGui::Text(m_item_name.c_str());
		ImGui::EndDragDropSource();
		m_p_owner_tree->SetDragStartItem(this);
	}

	//드랍 된 경우
	if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::GameObject))
		m_p_owner_tree->SetDroppedItem(this);
}

const bool GUI_TreeItem::CheckClickEmptySpace()
{
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_None))
	{
		//어떠한 아이템 위에도 커서가 올라가 있지 않은 상태에서 마우스 왼쪽 버튼을 눌렀을 경우
		if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_clicked_empty_space = true;
		}
	}

	return m_clicked_empty_space;
}

const bool GUI_TreeItem::CheckClickMouseLeftButton()
{
	//아이템 왼쪽 클릭 된 경우
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
		m_press_mouse_left_button = true;

	return m_press_mouse_left_button;
}
