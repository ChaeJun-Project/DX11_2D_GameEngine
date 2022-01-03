#include "stdafx.h"
#include "GUI_Hierarchy.h"

//Helper
#include "Helper/EditorHelper.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>
#include <DX11_2D_GameEngine_Lib/GameObject.h>

#include <DX11_2D_GameEngine_Lib/EventManager.h>


GUI_Hierarchy::GUI_Hierarchy(const std::string& hierarchy_title)
	:IGUI(hierarchy_title)
{
	m_p_clicked_func = std::bind
	(
		&GUI_Hierarchy::ClickedGameObject,
		this,
		std::placeholders::_1
	);

	m_p_drag_drop_func = std::bind
	(
		&GUI_Hierarchy::DragDropGameObject,
		this,
		std::placeholders::_1,
		std::placeholders::_2
	);
}

GUI_Hierarchy::~GUI_Hierarchy()
{
	m_p_current_scene.reset();
}

void GUI_Hierarchy::Initialize()
{
	UpdateTree();
}

void GUI_Hierarchy::Update()
{
	if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_H))
	{
		m_is_active = !m_is_active;
	}

	//Scene내의 GameObject 변경점이 있는 경우 
	if (EventManager::GetInstance()->IsUpdate())
	{
		UpdateTree();
	}
}

void GUI_Hierarchy::Render()
{
	ShowHierarchy();

	CheckClickRightButton();

	ShowMenuPopup();

	//현재 윈도우가 포커싱되었을 경우
	if (ImGui::IsWindowFocused())
		CheckEvnetKey();
}

void GUI_Hierarchy::UpdateTree()
{
	m_gui_tree.Clear();
	m_gui_tree.SetIsVisibleRoot(false);

	m_p_current_scene = SceneManager::GetInstance()->GetCurrentScene();

	auto p_root_tree_item = m_gui_tree.AddItem(nullptr, m_p_current_scene->GetSceneName(), PayLoadType::NONE, 0);

	const auto& root_game_object_vector = m_p_current_scene->GetAllParentGameObjects();
	for (const auto& root_game_object : root_game_object_vector)
		AddGameObject(p_root_tree_item, root_game_object);

	m_gui_tree.SetClickedCallBack(m_p_clicked_func);
	m_gui_tree.SetDragDropCallBack(m_p_drag_drop_func);
}

void GUI_Hierarchy::CheckClickRightButton()
{
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup))
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			ImGui::OpenPopup("Hierarchy Menu Popup");
	}
}

void GUI_Hierarchy::ShowHierarchy()
{
	//ImGuiTreeNodeFlags_DefaultOpen : 시작부터 하위노드를 다 보여주는 옵션
	if (ImGui::CollapsingHeader(m_p_current_scene->GetSceneName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		m_gui_tree.Update();
	}
}

void GUI_Hierarchy::ClickedGameObject(DWORD_PTR object_address)
{
	GameObject* p_selected_game_object = (GameObject*)object_address;

	SelectedGameObject(p_selected_game_object);
}

void GUI_Hierarchy::DragDropGameObject(DWORD_PTR p_dropped_item, DWORD_PTR p_drag_start_item)
{
	GUI_TreeItem* p_dest_item = (GUI_TreeItem*)p_dropped_item;
	GUI_TreeItem* p_src_item = (GUI_TreeItem*)p_drag_start_item;

	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Add_Child_Object;
	event_struct.object_address_1 = (GameObject*)p_dest_item->GetPayLoadData();
	event_struct.object_address_2 = (GameObject*)p_src_item->GetPayLoadData();

	EventManager::GetInstance()->AddEvent(event_struct);
}

void GUI_Hierarchy::CheckEvnetKey()
{
	if (KEY_DOWN(KeyCode::KEY_DELETE))
	{
		auto select_game_object = EditorHelper::GetInstance()->GetSelectedGameObject();
		if (select_game_object == nullptr)
			return;

		DeleteGameObject(select_game_object);

		EditorHelper::GetInstance()->SetSelectedGameObject(nullptr);
	}
}

void GUI_Hierarchy::DeleteGameObject(GameObject* game_object)
{
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Delete_Object;
	event_struct.object_address_1 = game_object;

	EventManager::GetInstance()->AddEvent(event_struct);
}


void GUI_Hierarchy::AddGameObject(GUI_TreeItem* p_tree_item, GameObject* game_object)
{
	if (game_object != nullptr && game_object->IsDead())
		return;

	std::string game_object_name = game_object->GetGameObjectName();
	auto p_current_tree_item = m_gui_tree.AddItem(p_tree_item, game_object_name, PayLoadType::GameObject, (DWORD_PTR)game_object);

	const auto& childs_vector = game_object->GetChilds();

	for (const auto& child_game_object : childs_vector)
		AddGameObject(p_current_tree_item, child_game_object);
}

void GUI_Hierarchy::ShowMenuPopup()
{
	if (!ImGui::BeginPopup("Hierarchy Menu Popup"))
		return;

	if (ImGui::BeginMenu("Create Object"))
	{
		if (ImGui::MenuItem("Empty"))
		{
			CreateGameObject();
		}

		ImGui::EndMenu();
	}

	ImGui::EndPopup();
}

void GUI_Hierarchy::SelectedGameObject(GameObject* game_object)
{
	EditorHelper::GetInstance()->SetSelectedGameObject(game_object);
}

void GUI_Hierarchy::CreateGameObject()
{
	auto p_new_game_object = new GameObject();
	p_new_game_object->AddComponent(new Transform());

	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Create_Object;
	event_struct.object_address_1 = p_new_game_object;
	event_struct.layer_index = 0;

	EventManager::GetInstance()->AddEvent(event_struct);
}


