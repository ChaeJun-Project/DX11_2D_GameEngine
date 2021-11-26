#include "stdafx.h"
#include "GUI_Hierarchy.h"

//Helper
#include "Helper/EditorHelper.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>


GUI_Hierarchy::GUI_Hierarchy(const std::string& hierarchy_title)
	:IGUI(hierarchy_title)
{
}

GUI_Hierarchy::~GUI_Hierarchy()
{
	m_p_current_scene.reset();

	m_hovered_game_object = nullptr;
	m_clicked_game_object = nullptr;
	m_empty_game_object = nullptr;
}

void GUI_Hierarchy::Update()
{
	if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_H))
	{
		m_is_active = !m_is_active;
	}
}

void GUI_Hierarchy::Render()
{
	ShowHierarchy();

	ClickedGameObject();

	ClickedCheck();

	ShowMenuPopup();
}

void GUI_Hierarchy::ShowHierarchy()
{
	m_p_current_scene = SceneManager::GetInstance()->GetCurrentScene();

	//ImGuiTreeNodeFlags_DefaultOpen : ���ۺ��� ������带 �� �����ִ� �ɼ�
	if (ImGui::CollapsingHeader(m_p_current_scene->GetSceneName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		const auto& root_game_object_vector = m_p_current_scene->GetAllParentGameObjects();
		for (const auto& root_game_object : root_game_object_vector)
			AddGameObject(root_game_object);
	}
}

void GUI_Hierarchy::AddGameObject(GameObject* game_object)
{
	if (game_object == nullptr)
		return;

	const auto& childs_vector = game_object->GetChilds();
	//ImGuiTreeNodeFlags_AllowItemOverlap: ���콺�� ����ٰ� ������ �ڵ����� �ڽ����� ������ִ� �÷���
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;
	//ImGuiTreeNodeFlags_OpenOnArrow : ȭ��ǥ�� on�������� open�ϴ� �÷���
	//�ڽ��� ���ٸ�
	flags |= childs_vector.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;

	if (const auto selected_actor = EditorHelper::GetInstance()->GetSelectedGameObject())
	{
		const auto is_selected = selected_actor->GetObjectID() == game_object->GetObjectID();
		flags |= is_selected ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
	}

	//��� �߰��ؼ� ȭ�鿡 �����ִ� ����
	const auto is_node_open = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(game_object->GetObjectID())), flags, game_object->GetObjectName().c_str());

	if (m_clicked_game_object != nullptr && EditorHelper::GetInstance()->GetSelectedGameObject() != nullptr)
	{
		if (EditorHelper::GetInstance()->GetSelectedGameObject()->GetObjectID() == m_clicked_game_object->GetObjectID())
			ImGui::SetItemDefaultFocus();
	}

	if (is_node_open)
	{
		for (const auto& child_game_object : childs_vector)
			AddGameObject(child_game_object);
		//��� �߰��� ������
		ImGui::TreePop();
	}

	//���� ���콺 Ŀ���� ������Ʈ �ؽ�Ʈ�� �ö� ���
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly))
		m_hovered_game_object = game_object;
}

void GUI_Hierarchy::ClickedGameObject()
{
	//Hierarchy Window�� ���콺�� �ö� �ִ��� Ȯ���ϴ� ����
	const auto is_window_hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup);
	//���콺 ���� Ŭ��
	const auto is_left_click = ImGui::IsMouseClicked(0);
	//���콺 ������ Ŭ��
	const auto is_right_click = ImGui::IsMouseClicked(1);

	if (!is_window_hovered)
		return;


	//���� Ŭ��
	if (is_left_click)
	{
		//���� ���콺 Ŀ���� �ö� ������Ʈ�� �ִٸ�
		if (m_hovered_game_object != nullptr)
			m_clicked_game_object = m_hovered_game_object;

		//���� ���콺 Ŀ���� �ö� ������Ʈ�� ���ٸ�
		else
		{
			SelectedGameObject(m_empty_game_object);
		}
	}

	//������ Ŭ��
	if (is_right_click)
	{
		//���� ���õǾ��� ������Ʈ�� �ִٸ�
		if (m_hovered_game_object != nullptr)
			m_clicked_game_object = m_hovered_game_object;

		ImGui::OpenPopup("Hierarchy Menu Popup");
	}

	//���콺 ���� �Ǵ� ������ Ŭ���� ���� �� ���� ���콺 Ŀ���� �ö� ������Ʈ�� ���ٸ�
	if ((is_left_click || is_right_click) && m_hovered_game_object == nullptr)
	{
		SelectedGameObject(m_empty_game_object);
	}
}

void GUI_Hierarchy::ClickedCheck()
{
	if ((ImGui::IsMouseReleased(0) || ImGui::IsMouseReleased(1)) && m_clicked_game_object != nullptr)
	{
		if (m_hovered_game_object != nullptr && m_hovered_game_object->GetObjectID() == m_clicked_game_object->GetObjectID())
			SelectedGameObject(m_clicked_game_object);

		m_hovered_game_object = nullptr;
		m_clicked_game_object = nullptr;
	}
}

void GUI_Hierarchy::ShowMenuPopup()
{
	if (!ImGui::BeginPopup("Hierarchy Menu Popup"))
		return;

	if (ImGui::BeginMenu("Create 2D Object"))
	{
		if (ImGui::MenuItem("Sprite"))
		{

		}

		ImGui::EndMenu();
	}

	ImGui::EndPopup();
}

void GUI_Hierarchy::SelectedGameObject(GameObject* game_object)
{
	EditorHelper::GetInstance()->SetSelectedGameObject(game_object);
}


