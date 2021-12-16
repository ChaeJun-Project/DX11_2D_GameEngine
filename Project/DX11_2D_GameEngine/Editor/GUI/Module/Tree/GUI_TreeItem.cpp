#include "stdafx.h"
#include "GUI_TreeItem.h"

#include "GUI_Tree.h"

GUI_TreeItem::GUI_TreeItem(GUI_TreeItem* p_parent, const std::string& item_name, const PayLoadType& pay_load_type, DWORD_PTR object_address)
	: m_p_parent(p_parent),
	m_item_name(item_name)
{
	m_pay_load.type = pay_load_type;
	m_pay_load.data = object_address;
}

GUI_TreeItem::~GUI_TreeItem()
{
	m_p_child_vector.clear();
	m_p_child_vector.shrink_to_fit();
}

void GUI_TreeItem::Update()
{
	//TreeItem�� �ڽ��� ������ �ִ� ��� ���� Ŭ�� or ���� ȭ��ǥ�� ������ ��� ��ħ
	int flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

	//TreeItem�� �ڽ��� ������ ���� ���� ���
	if (m_p_child_vector.empty())
		flags |= ImGuiTreeNodeFlags_Leaf;  //�ش� TreeItem�� ������ �׸�

	//���� ���õ� �������� �ڽ��� ���
	if (m_p_owner_tree->GetSelectedItem() == this)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	//���� �巡�� ���� �������� �ڽ��� ���
	if (m_p_owner_tree->GetDragStartItem() == this)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool press_mouse_left_button = false;

	auto object = (DX11Obejct*)(std::get<DWORD_PTR>(m_pay_load.data));

	if (ImGui::TreeNodeEx((const void*)(object->GetObjectID()), flags, m_item_name.c_str()))
	{
		DragAndDrop();

		press_mouse_left_button = CheckClickMouseLeftButton();

		for (auto& child_tree_item : m_p_child_vector)
			child_tree_item->Update();

		//��� �߰��� ������
		ImGui::TreePop();
	}

	else
	{
		DragAndDrop();

		press_mouse_left_button = CheckClickMouseLeftButton();
	}

	if (press_mouse_left_button)
	{
		m_p_owner_tree->ExcuteClickedCallBack(this);
	}

	//���콺 ���� �� ���� ���� �ǵ��� ����
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		m_press_mouse_left_button = false;
	}
}

void GUI_TreeItem::DragAndDrop()
{
	//�巡�� ����
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		DragDropEvent::CreateDragDropPayLoad(m_pay_load);
		ImGui::Text(m_item_name.c_str());
		ImGui::EndDragDropSource();
		m_p_owner_tree->SetDragStartItem(this);
	}

	//��� �� ���
	if (ImGui::BeginDragDropTarget())
	{
		auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(m_pay_load.type);
		if (pay_load != nullptr)
		{
			m_p_owner_tree->SetDroppedItem(this);
		}

		ImGui::EndDragDropTarget();
	}
}

const bool GUI_TreeItem::CheckClickMouseLeftButton()
{
	//������ ���� Ŭ�� �� ���
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_None) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		m_press_mouse_left_button = true;
		return true;
	}

	return false;
}