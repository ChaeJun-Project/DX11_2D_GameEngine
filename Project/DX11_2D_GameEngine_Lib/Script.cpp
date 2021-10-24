#include "stdafx.h"
#include "Script.h"

Script::Script()
	:IComponent(ComponentType::Script)
{
}

void Script::Instantiate(std::shared_ptr<Prefab>& p_game_object_prefab, const Vector3& position, const UINT& layer_index)
{
	GameObject* p_game_object = p_game_object_prefab->Instantiate();

	auto transform = p_game_object->GetComponent<Transform>();

	if (transform == nullptr)
		assert(false);

	transform->SetTranslation(position);
	CreateGameObject(p_game_object, layer_index);
}

void Script::Destroy(GameObject* p_delete_game_object)
{
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Delete_Object;
	event_struct.object_address = (DWORD_PTR)(p_delete_game_object);

	EventManager::GetInstance()->AddEvent(event_struct);
}

void Script::CreateGameObject(GameObject* p_new_game_object, const UINT& layer_index)
{
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Create_Object;
	event_struct.object_address = (DWORD_PTR)(p_new_game_object);
	event_struct.layer_index = layer_index;

	EventManager::GetInstance()->AddEvent(event_struct);
}

