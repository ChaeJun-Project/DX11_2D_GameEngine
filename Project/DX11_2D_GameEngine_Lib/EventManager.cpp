#include "stdafx.h"
#include "EventManager.h"

#include "Scene.h"
#include "GameObject.h"

EventManager::~EventManager()
{
	//Event Queue ��� ������ ���� �� �޸� ����
	ClearQueue<EventStruct>(this->m_event_struct_queue);
	ClearQueue<GameObject*>(this->m_p_dead_game_object_queue);
}

void EventManager::Update()
{
	//���� �����ӿ��� ����ص� Dead Game Object���� ����
	while (!this->m_p_dead_game_object_queue.empty())
	{
		SAFE_DELETE(this->m_p_dead_game_object_queue.front());
		this->m_p_dead_game_object_queue.pop();
	}

	//Event ó��
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
		this->m_p_dead_game_object_queue.push(p_dead_game_object); //Dead Game Object ť�� ���
	}
	break;
	}
}

void EventManager::AddEvent(const EventStruct& event_struct)
{
	this->m_event_struct_queue.push(event_struct);
}

