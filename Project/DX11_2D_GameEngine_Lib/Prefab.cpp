#include "stdafx.h"
#include "Prefab.h"

#include "GameObject.h"

Prefab::Prefab(const std::string& resource_name)
	:IResource(ResourceType::Prefab, resource_name)
{
}

Prefab::Prefab(GameObject* p_prototype_game_object)
	: IResource(ResourceType::Prefab, p_prototype_game_object->GetObjectName())
{
	this->m_p_prototype_game_object = p_prototype_game_object;
}

Prefab::~Prefab()
{
    //SAFE_DELETE(m_p_prototype_game_object);
}

GameObject* Prefab::Instantiate()
{
	return this->m_p_prototype_game_object->Clone();
}
