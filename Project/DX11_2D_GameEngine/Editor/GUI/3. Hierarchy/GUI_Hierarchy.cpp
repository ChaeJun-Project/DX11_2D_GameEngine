#include "stdafx.h"
#include "GUI_Hierarchy.h"

//Helper
#include "Helper/EditorHelper.h"

#include <DX11_2D_GameEngine_Lib/EventManager.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>

GUI_Hierarchy::GUI_Hierarchy(const std::string& hierarchy_title)
	:IGUI(hierarchy_title)
{
	m_scene_hierarchy_tree = std::make_unique<GUI_Tree>();

	m_p_clicked_empty_space = std::bind
	(
		&GUI_Hierarchy::ClickEmptySpace,
		this
	);

	m_p_clicked_func_2 = std::bind
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
	m_scene_hierarchy_tree->Clear();
	m_scene_hierarchy_tree.reset();

	m_p_current_scene.reset();
}

void GUI_Hierarchy::Initialize()
{
	UpdateTree();
}

void GUI_Hierarchy::Update()
{
	//Scene내의 GameObject 변경점이 있는 경우 
	if (EVENT_MANAGER->IsUpdate())
	{
		UpdateTree();
	}
}

void GUI_Hierarchy::Render()
{
	ShowHierarchy();

	if (SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Stop)
	{
		CheckClickRightButton();

		ShowMenuPopup();

		//현재 윈도우가 포커싱되었을 경우
		if (ImGui::IsWindowFocused())
			CheckEvnetKey();
	}
}

void GUI_Hierarchy::UpdateTree()
{
	m_scene_hierarchy_tree->Clear();
	m_scene_hierarchy_tree->SetIsVisibleRoot(false);

	m_p_current_scene = SCENE_MANAGER->GetCurrentScene();

	PayLoad pay_load;
	pay_load.type = PayLoadType::None;
	pay_load.data = 0;

	auto p_root_tree_item = m_scene_hierarchy_tree->AddItem(nullptr, m_p_current_scene->GetSceneName(), pay_load, true);

	const auto& root_game_object_vector = m_p_current_scene->GetAllParentGameObjects();
	for (const auto& root_game_object : root_game_object_vector)
		AddGameObject(p_root_tree_item, root_game_object);

	m_scene_hierarchy_tree->SetClickedEmptySpace(m_p_clicked_empty_space);
	m_scene_hierarchy_tree->SetClickedCallBack2(m_p_clicked_func_2);
	m_scene_hierarchy_tree->SetDragDropCallBack(m_p_drag_drop_func);
}

void GUI_Hierarchy::CheckClickRightButton()
{
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_None))
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
		//Make Prefab -> GameObject
		//드랍 된 경우
		if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Prefab))
		{

		}

		m_scene_hierarchy_tree->Update();
	}
}

void GUI_Hierarchy::ClickEmptySpace()
{
	SelectedGameObject(nullptr);
}

void GUI_Hierarchy::ClickedGameObject(DWORD_PTR object_address)
{
	GameObject* p_selected_game_object = (GameObject*)object_address;

	SelectedGameObject(p_selected_game_object);
}

void GUI_Hierarchy::DragDropGameObject(DWORD_PTR p_dropped_item, DWORD_PTR p_drag_start_item)
{
	if (SCENE_MANAGER->GetEditorState() != EditorState::EditorState_Stop)
		return;

	GUI_TreeItem* p_dest_item = (GUI_TreeItem*)p_dropped_item;
	GUI_TreeItem* p_src_item = (GUI_TreeItem*)p_drag_start_item;

	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Add_Child_Object;
	event_struct.object_address_1 = (GameObject*)p_dest_item->GetPayLoadData();
	event_struct.object_address_2 = (GameObject*)p_src_item->GetPayLoadData();

	EVENT_MANAGER->AddEvent(event_struct);
}

void GUI_Hierarchy::CheckEvnetKey()
{
	if (KEY_DOWN(KeyCode::KEY_DELETE))
	{
		auto select_game_object = EDITOR_HELPER->GetSelectedGameObject();
		if (select_game_object == nullptr)
			return;

		DeleteGameObject(select_game_object);

		EDITOR_HELPER->SetSelectedGameObject(nullptr);
	}
}

void GUI_Hierarchy::DeleteGameObject(GameObject* game_object)
{
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Delete_Object;
	event_struct.object_address_1 = game_object;

	EVENT_MANAGER->AddEvent(event_struct);
}


void GUI_Hierarchy::AddGameObject(GUI_TreeItem* p_tree_item, GameObject* game_object)
{
	if (p_tree_item == nullptr || game_object == nullptr || game_object->IsDead())
		return;

	std::string game_object_name = game_object->GetGameObjectName();

	PayLoad pay_load;
	pay_load.type = PayLoadType::GameObject;
	pay_load.data = (DWORD_PTR)game_object;

	auto p_current_tree_item = m_scene_hierarchy_tree->AddItem(p_tree_item, game_object_name, pay_load, true);

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
	EDITOR_HELPER->SetSelectedGameObject(game_object);
}

void GUI_Hierarchy::CreateGameObject()
{
	//Create New GameObject
	auto p_new_game_object = new GameObject();
	p_new_game_object->SetGameObjectName("GameObject");
	p_new_game_object->AddComponent(ComponentType::Transform);

	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Create_Object;
	event_struct.object_address_1 = p_new_game_object;

	EVENT_MANAGER->AddEvent(event_struct);

	//현재 선택된 GameObject가 있다면
	//Add Child GameObject
	auto p_parent_game_object = EDITOR_HELPER->GetSelectedGameObject();

	if (p_parent_game_object != nullptr)
	{
		ZeroMemory(&event_struct, sizeof(EventStruct));

		event_struct.event_type = EventType::Add_Child_Object;
		event_struct.object_address_1 = p_parent_game_object;
		event_struct.object_address_2 = p_new_game_object;

		EVENT_MANAGER->AddEvent(event_struct);
	}
}


