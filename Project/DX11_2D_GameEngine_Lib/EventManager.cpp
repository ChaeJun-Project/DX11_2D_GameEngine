#include "stdafx.h"
#include "EventManager.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "GameObject.h"

EventManager::~EventManager()
{
	//Event Queue 모든 데이터 삭제 및 메모리 해제
	ClearQueue<EventStruct>(m_event_struct_queue);
	ClearQueue<GameObject*>(m_p_dead_game_object_queue);
}

void EventManager::Update()
{
	m_is_update = false;

	//이전 프레임에서 등록해둔 Dead Game Object들을 삭제
	while (!m_p_dead_game_object_queue.empty())
	{
		auto p_dead_game_object = m_p_dead_game_object_queue.front();
		m_p_dead_game_object_queue.pop();

		p_dead_game_object->DetachFromParent(); //부모 오브젝트가 있다면 연결 해제

		auto p_current_scene = SCENE_MANAGER->GetCurrentScene();
		p_current_scene->DeregisterGameObject(p_dead_game_object, true); //현재 Scene에서 등록 해제(Layer 포함)

		SAFE_DELETE(p_dead_game_object);

		m_is_update = true;
	}

	//Event 처리
	while (!m_event_struct_queue.empty())
	{
		Excute(m_event_struct_queue.front());
		m_event_struct_queue.pop();
	}
}

//<summary>
//object_address_1: 이벤트 처리에 해당하는 GameObject의 주소
//object_address_2: 자식 GameObject를 연결할 때 필요한 자식 GameObject의 주소
//</summary>
void EventManager::Excute(const EventStruct& event_struct)
{
	switch (event_struct.event_type)
	{
	case EventType::Create_Object:
	{
		CreateGameObject(event_struct);
	}
	break;
	case EventType::Delete_Object:
	{
		DeleteGameObject(event_struct);
	}
	break;
	case EventType::Add_Child_Object:
	{
		AddChildGameObject(event_struct);
	}
	break;
	case EventType::Detach_Child_Object:
	{
		DetachChildGameObject(event_struct);
	}
	break;
	case EventType::Scene_Change:
	{
		ChangeScene(event_struct);
	}
	break;
	case EventType::Update:
	{
		m_is_update = true;
	}
	break;
	}
}

void EventManager::CreateGameObject(const EventStruct& event_struct)
{
	auto p_new_game_object = std::get<GameObject*>(event_struct.object_address_1);
	SCENE_MANAGER->GetCurrentScene()->RegisterGameObject(p_new_game_object);

	//Game Play 중이라면
	if (SCENE_MANAGER->GetClientState() == 1 || SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Play)
	{
		p_new_game_object->Awake();

		if (p_new_game_object->GetIsActive())
			p_new_game_object->Start();
	}

	m_is_update = true;
}

void EventManager::DeleteGameObject(const EventStruct& event_struct)
{
	auto p_dead_game_object = std::get<GameObject*>(event_struct.object_address_1);
	p_dead_game_object->SetDead(); //m_dead_check = true
	m_p_dead_game_object_queue.push(p_dead_game_object); //Dead Game Object 큐에 등록
}

void EventManager::AddChildGameObject(const EventStruct& event_struct)
{
	auto p_parent_game_object = std::get<GameObject*>(event_struct.object_address_1);
	auto p_child_game_object = std::get<GameObject*>(event_struct.object_address_2);

	//현재 Scene
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	//이미 해당 GameObject를 자식으로 소유하고 있는 경우
	if (p_parent_game_object->GetHasChild(p_child_game_object))
		return;

	current_scene->DeregisterGameObject(p_child_game_object); //현재 Scene에서 등록 해제(Layer 미포함)

	//연결할 자식 GameObject가 부모 GameObject를 소유하고 있는 경우
	if (p_child_game_object->HasParent())
		p_child_game_object->DetachFromParent(); //기존 부모로부터 분리

	//새로운 자식 GameObject를 부모 GameObject에 연결
	p_parent_game_object->AddChild(p_child_game_object);

	//필요없는 부분
	////새로운 자식 GameObject 연결 후 현재 Scene에서 등록 해제(Layer 포함)
	//current_scene->DeregisterGameObject(p_parent_game_object); 
	////새로운 자식 GameObject 연결 후 현재 Scene에 다시 등록(Layer 포함)
	//current_scene->RegisterGameObject(p_parent_game_object);

	m_is_update = true;
}

void EventManager::DetachChildGameObject(const EventStruct& event_struct)
{
	//object_address_1은 nullptr
	auto p_game_object = std::get<GameObject*>(event_struct.object_address_2);

	//현재 Scene
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	//현재 GameObject가 부모 GameObject를 가지고 있는 경우
	if (p_game_object->HasParent())
	{
		current_scene->DeregisterGameObject(p_game_object); //현재 Scene에서 등록 해제(Layer 미포함)
		p_game_object->DetachFromParent(); //현재 부모 GameObject로부터 분리
	}

	//현재 GameObject가 부모 GameObject를 가지고 있지 않은 경우
	//아무런 변화 없음
	else
		return;

	current_scene->RegisterGameObject(p_game_object); //현재 Scene에 다시 등록(최상위 부모 GameObject로, Layer 포함)

	m_is_update = true;
}

void EventManager::ChangeScene(const EventStruct& event_struct)
{
	RENDER_MANAGER->ClearCameraAndLight(); //등록된 Camera 및 Light 제거

	auto next_scene = std::get<std::shared_ptr<Scene>>(event_struct.object_address_1);
	SCENE_MANAGER->SetCurrentScene(next_scene);

	m_is_update = true;
}

void EventManager::AddEvent(const EventStruct& event_struct)
{
	m_event_struct_queue.push(event_struct);
}

