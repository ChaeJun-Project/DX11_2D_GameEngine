#include "stdafx.h"
#include "Prefab.h"

#include "GameObject.h"

Prefab::Prefab(GameObject* p_prototype_game_object)
	: IResource(ResourceType::Prefab, p_prototype_game_object->GetGameObjectName())
{
	m_p_prototype_game_object = p_prototype_game_object;
}

Prefab::~Prefab()
{
    SAFE_DELETE(m_p_prototype_game_object);
}

GameObject* Prefab::Instantiate()
{
	return m_p_prototype_game_object->Clone();
}

void Prefab::LoadFromFile(const std::string& texture_path)
{
}

void Prefab::SaveFile(const std::string& texture_path)
{
}
