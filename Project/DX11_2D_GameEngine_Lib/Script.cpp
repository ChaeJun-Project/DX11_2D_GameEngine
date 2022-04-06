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

Script::~Script()
{
	m_script_param_vector.clear();
	m_script_param_vector.shrink_to_fit();
}

GameObject* Script::Instantiate(const std::shared_ptr<Prefab>& p_game_object_prefab, const Vector3& position)
{
	GameObject* p_game_object = p_game_object_prefab->Instantiate();

	if (p_game_object == nullptr)
		return nullptr;

	auto transform = p_game_object->GetComponent<Transform>();
	if (position != Vector3::Zero)
		transform->SetTranslation(position);

	CreateGameObject(p_game_object);

	return p_game_object;
}

void Script::OnDestroy(GameObject* p_delete_game_object)
{
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Delete_Object;
	event_struct.object_address_1 = p_delete_game_object;

	EVENT_MANAGER->AddEvent(event_struct);
}

void Script::CreateGameObject(GameObject* p_new_game_object)
{
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Create_Object;
	event_struct.object_address_1 = p_new_game_object;

	EVENT_MANAGER->AddEvent(event_struct);
}

void Script::SetIsActive(const bool& is_active)
{
	if (SCENE_MANAGER->GetClientState() == 1 || SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Play)
	{
		if (m_is_active != is_active)
		{
			if (is_active)
			{
				OnEnable();
				if (m_start_func_call_count == 1)
				{
					Start();
					++m_start_func_call_count;
				}
			}

			else
				OnDisable();
		}
	}

	m_is_active = is_active;
}

void Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Script Name
	fprintf(p_file, "[Name]\n");
	fprintf(p_file, "%s\n", m_script_name.c_str());
}