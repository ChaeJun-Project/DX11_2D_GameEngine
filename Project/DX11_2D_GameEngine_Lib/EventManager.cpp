#include "stdafx.h"
#include "EventManager.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "GameObject.h"

EventManager::~EventManager()
{
	//Event Queue ��� ������ ���� �� �޸� ����
	ClearQueue<EventStruct>(m_event_struct_queue);
	ClearQueue<GameObject*>(m_p_dead_game_object_queue);
}

void EventManager::Update()
{
	m_is_update = false;

	//���� �����ӿ��� ����ص� Dead Game Object���� ����
	while (!m_p_dead_game_object_queue.empty())
	{
		auto p_dead_game_object = m_p_dead_game_object_queue.front();
		p_dead_game_object->DetachFromParent(); //�θ� ������Ʈ�� �ִٸ� ���� ����
		SAFE_DELETE(p_dead_game_object);

		m_p_dead_game_object_queue.pop();

		m_is_update = true;
	}

	//Event ó��
	while (!m_event_struct_queue.empty())
	{
		Excute(m_event_struct_queue.front());
		m_event_struct_queue.pop();
	}
}

//<summary>
//object_address_1: �̺�Ʈ ó���� �ش��ϴ� GameObject�� �ּ�
//object_address_2: �ڽ� GameObject�� ������ �� �ʿ��� �ڽ� GameObject�� �ּ�
//</summary>
void EventManager::Excute(const EventStruct& event_struct)
{
	switch (event_struct.event_type)
	{
	case EventType::Create_Object:
	{
		auto p_new_game_object = (GameObject*)(event_struct.object_address_1);
		SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(p_new_game_object);

		m_is_update = true;
	}
	break;
	case EventType::Delete_Object:
	{
		auto p_dead_game_object = (GameObject*)(event_struct.object_address_1);
		p_dead_game_object->SetDead(); //m_dead_check = true
		m_p_dead_game_object_queue.push(p_dead_game_object); //Dead Game Object ť�� ���
	}
	break;
	case EventType::Add_Child_Object:
	{
		auto p_parent_game_object = (GameObject*)(event_struct.object_address_1);
		auto p_child_game_object = (GameObject*)(event_struct.object_address_2);

		//�̹� �ش� GameObject�� �ڽ����� �����ϰ� �ִ� ���
		if (p_parent_game_object->GetHasChild(p_child_game_object))
			return;

		//������ �ڽ� GameObject�� �θ� GameObject�� �����ϰ� �ִ� ���
		if (p_child_game_object->HasParent())
			p_child_game_object->DetachFromParent(); //�θ�κ��� �и�

		//������ �ڽ� GameObject�� �ֻ��� �θ��� ���
		else
		{
			auto layer_index = p_child_game_object->GetGameObjectLayer();
			if (layer_index != -1)
			{
				auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
				auto layer = current_scene->GetLayer(static_cast<UINT>(layer_index));
				layer->DeregisterFromParentGameObject(p_child_game_object);
			}
		}

		p_parent_game_object->AddChild(p_child_game_object);

		m_is_update = true;
	}
	break;
	case EventType::Scene_Change:
	{
	   //TODO
	}
	break;
	}
}

void EventManager::AddEvent(const EventStruct& event_struct)
{
	m_event_struct_queue.push(event_struct);
}

