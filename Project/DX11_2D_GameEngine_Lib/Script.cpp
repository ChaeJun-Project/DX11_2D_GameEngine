#include "stdafx.h"
#include "Script.h"

Script::Script()
	:IComponent(ComponentType::Script)
{
}

void Script::Update()
{
}

void Script::FinalUpdate()
{
}

void Script::CreateGameObject(GameObject* p_new_game_object, const int& layer_index)
{
}

void Script::CreateGameObject(GameObject* p_new_game_object, const Vector3& position, const int& layer_index)
{
}

void Script::DeleteGameObject(GameObject* p_delete_game_object)
{
}
