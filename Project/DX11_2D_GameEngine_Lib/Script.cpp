#include "stdafx.h"
#include "Script.h"

#include "Prefab.h"

#include "GameObject.h"
#include "Transform.h"

Script::Script(const std::string& script_name)
	:IComponent(ComponentType::Script),
	m_script_name(script_name)
{
}

Script::Script(const Script& origin)
	: IComponent(origin.m_component_type)
{
}

Script::~Script()
{
	m_script_param_vector.clear();
	m_script_param_vector.shrink_to_fit();
}

GameObject* Script::Instantiate(std::shared_ptr<Prefab>& p_game_object_prefab, const Vector3& position, const bool& use_event)
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
		CreateGameObject(p_game_object);

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

void Script::CreateGameObject(GameObject* p_new_game_object)
{
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Create_Object;
	event_struct.object_address_1 = p_new_game_object;

	EventManager::GetInstance()->AddEvent(event_struct);
}

void Script::SaveToScene(FILE* p_file)
{
    __super::SaveToScene(p_file); //IComponent

	//Script Name
	fprintf(p_file, "[Name]\n");
	fprintf(p_file, "%s\n", m_script_name.c_str());
}