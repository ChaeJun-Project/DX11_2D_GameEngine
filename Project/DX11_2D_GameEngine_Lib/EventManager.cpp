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
		m_p_dead_game_object_queue.pop();

		p_dead_game_object->DetachFromParent(); //�θ� ������Ʈ�� �ִٸ� ���� ����

		auto p_current_scene = SCENE_MANAGER->GetCurrentScene();
		p_current_scene->DeregisterGameObject(p_dead_game_object, true); //���� Scene���� ��� ����(Layer ����)

		SAFE_DELETE(p_dead_game_object);

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

	//Game Play ���̶��
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
	m_p_dead_game_object_queue.push(p_dead_game_object); //Dead Game Object ť�� ���
}

void EventManager::AddChildGameObject(const EventStruct& event_struct)
{
	auto p_parent_game_object = std::get<GameObject*>(event_struct.object_address_1);
	auto p_child_game_object = std::get<GameObject*>(event_struct.object_address_2);

	//���� Scene
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	//�̹� �ش� GameObject�� �ڽ����� �����ϰ� �ִ� ���
	if (p_parent_game_object->GetHasChild(p_child_game_object))
		return;

	current_scene->DeregisterGameObject(p_child_game_object); //���� Scene���� ��� ����(Layer ������)

	//������ �ڽ� GameObject�� �θ� GameObject�� �����ϰ� �ִ� ���
	if (p_child_game_object->HasParent())
		p_child_game_object->DetachFromParent(); //���� �θ�κ��� �и�

	//���ο� �ڽ� GameObject�� �θ� GameObject�� ����
	p_parent_game_object->AddChild(p_child_game_object);

	//�ʿ���� �κ�
	////���ο� �ڽ� GameObject ���� �� ���� Scene���� ��� ����(Layer ����)
	//current_scene->DeregisterGameObject(p_parent_game_object); 
	////���ο� �ڽ� GameObject ���� �� ���� Scene�� �ٽ� ���(Layer ����)
	//current_scene->RegisterGameObject(p_parent_game_object);

	m_is_update = true;
}

void EventManager::DetachChildGameObject(const EventStruct& event_struct)
{
	//object_address_1�� nullptr
	auto p_game_object = std::get<GameObject*>(event_struct.object_address_2);

	//���� Scene
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	//���� GameObject�� �θ� GameObject�� ������ �ִ� ���
	if (p_game_object->HasParent())
	{
		current_scene->DeregisterGameObject(p_game_object); //���� Scene���� ��� ����(Layer ������)
		p_game_object->DetachFromParent(); //���� �θ� GameObject�κ��� �и�
	}

	//���� GameObject�� �θ� GameObject�� ������ ���� ���� ���
	//�ƹ��� ��ȭ ����
	else
		return;

	current_scene->RegisterGameObject(p_game_object); //���� Scene�� �ٽ� ���(�ֻ��� �θ� GameObject��, Layer ����)

	m_is_update = true;
}

void EventManager::ChangeScene(const EventStruct& event_struct)
{
	RENDER_MANAGER->ClearCameraAndLight(); //��ϵ� Camera �� Light ����

	auto next_scene = std::get<std::shared_ptr<Scene>>(event_struct.object_address_1);
	SCENE_MANAGER->SetCurrentScene(next_scene);

	m_is_update = true;
}

void EventManager::AddEvent(const EventStruct& event_struct)
{
	m_event_struct_queue.push(event_struct);
}

