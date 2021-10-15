#include "stdafx.h"
#include "EventManager.h"

#include "Scene.h"
#include "GameObject.h"

EventManager::~EventManager()
{
	//Event Queue 모든 데이터 삭제 및 메모리 해제
	ClearQueue<EventStruct>(this->m_event_struct_queue);
	ClearQueue<GameObject*>(this->m_p_dead_game_object_queue);
}

void EventManager::Update()
{
	//이전 프레임에서 등록해둔 Dead Game Object들을 삭제
	while (!this->m_p_dead_game_object_queue.empty())
	{
		SAFE_DELETE(this->m_p_dead_game_object_queue.front());
		this->m_p_dead_game_object_queue.pop();
	}

	//Event 처리
	while (!this->m_event_struct_queue.empty())
	{
		Excute(this->m_event_struct_queue.front());
		this->m_event_struct_queue.pop();
	}
}

void EventManager::Excute(const EventStruct& event_struct)
{
	switch (event_struct.event_type)
	{
	case EventType::Create_Object:
	{
		GameObject* p_new_game_object = (GameObject*)(event_struct.object_address);
		SceneManager::GetInstance()->GetCurrentScene()->AddGameObject
		(
			std::make_shared<GameObject>(*p_new_game_object),
			static_cast<UINT>(event_struct.layer_index),
			false
		);
	}
	break;
	case EventType::Delete_Object:
	{
		GameObject* p_dead_game_object = (GameObject*)(event_struct.object_address);
		p_dead_game_object->SetDead(); //m_dead_check = true
		this->m_p_dead_game_object_queue.push(p_dead_game_object); //Dead Game Object 큐에 등록
	}
	break;
	}
}

void EventManager::AddEvent(const EventStruct& event_struct)
{
	this->m_event_struct_queue.push(event_struct);
}

