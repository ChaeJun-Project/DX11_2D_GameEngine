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
		p_dead_game_object->DetachFromParent(); //부모 오브젝트가 있다면 연결 해제

		auto p_current_scene = SceneManager::GetInstance()->GetCurrentScene();
		p_current_scene->DeregisterGameObject(p_dead_game_object);

		SAFE_DELETE(p_dead_game_object);

		m_p_dead_game_object_queue.pop();

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
		auto p_new_game_object = std::get<GameObject*>(event_struct.object_address_1);
		SceneManager::GetInstance()->GetCurrentScene()->RegisterGameObject(p_new_game_object);

		m_is_update = true;
	}
	break;
	case EventType::Delete_Object:
	{
		auto p_dead_game_object = std::get<GameObject*>(event_struct.object_address_1);
		p_dead_game_object->SetDead(); //m_dead_check = true
		m_p_dead_game_object_queue.push(p_dead_game_object); //Dead Game Object 큐에 등록
	}
	break;
	case EventType::Add_Child_Object:
	{
		auto p_parent_game_object = std::get<GameObject*>(event_struct.object_address_1);
		auto p_child_game_object = std::get<GameObject*>(event_struct.object_address_2);

		//이미 해당 GameObject를 자식으로 소유하고 있는 경우
		if (p_parent_game_object->GetHasChild(p_child_game_object))
			return;

		//연결할 자식 GameObject가 부모 GameObject를 소유하고 있는 경우
		if (p_child_game_object->HasParent())
			p_child_game_object->DetachFromParent(); //부모로부터 분리

		//연결할 자식 GameObject가 최상위 부모인 경우
		else
		{
			auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
			current_scene->DeregisterFromParentGameObject(p_child_game_object);
		}

		p_parent_game_object->AddChild(p_child_game_object);

		m_is_update = true;
	}
	break;
	case EventType::Scene_Change:
	{
	   auto next_scene = std::get<std::shared_ptr<Scene>>(event_struct.object_address_1);
	   SceneManager::GetInstance()->SetCurrentScene(next_scene);

	   m_is_update = true;
	}
	break;
	}
}

void EventManager::AddEvent(const EventStruct& event_struct)
{
	m_event_struct_queue.push(event_struct);
}

