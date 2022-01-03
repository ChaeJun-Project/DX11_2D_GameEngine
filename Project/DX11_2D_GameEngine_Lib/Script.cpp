#include "stdafx.h"
#include "Script.h"

Script::Script()
	:IComponent(ComponentType::Script)
{
}

Script::Script(const Script& origin)
    : IComponent(origin.GetComponentType())
{
}

GameObject* Script::Instantiate(std::shared_ptr<Prefab>& p_game_object_prefab, const Vector3& position, const UINT& layer_index, const bool& use_event)
{
	GameObject* p_game_object = p_game_object_prefab->Instantiate();

	auto transform = p_game_object->GetComponent<Transform>();

	if (transform == nullptr)
	{
		assert(false);
		return nullptr;
	}

	transform->SetTranslation(position);

	if (use_event)
		CreateGameObject(p_game_object, layer_index);

	return p_game_object;
}

void Script::Destroy(GameObject* p_delete_game_object)
{
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Delete_Object;
	event_struct.object_address_1 = p_delete_game_object;

	EventManager::GetInstance()->AddEvent(event_struct);
}

void Script::CreateGameObject(GameObject* p_new_game_object, const UINT& layer_index)
{
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Create_Object;
	event_struct.object_address_1 = p_new_game_object;
	event_struct.layer_index = layer_index;

	EventManager::GetInstance()->AddEvent(event_struct);
}

