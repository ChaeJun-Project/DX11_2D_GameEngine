#include "stdafx.h"
#include "Prefab.h"

#include "GameObject.h"
#include "IComponent.h"
#include "Script.h"

//<summary>
//Client의 ClientSceneManager 클래스의 SaveGameObject, LoadGameObject 함수를 바인딩 하여 사용
//</summary>
SaveGameObjectFunc Prefab::p_save_game_object_func = nullptr;
LoadGameObjectFunc Prefab::p_load_game_object_func = nullptr;

Prefab::Prefab(const std::string& prefab_resource_name)
	: IResource(ResourceType::Prefab, prefab_resource_name)
{
}

Prefab::Prefab(GameObject* p_prototype_game_object)
	: IResource(ResourceType::Prefab, p_prototype_game_object->GetGameObjectName())
{
	m_p_prototype_game_object = p_prototype_game_object->Clone();
}

Prefab::~Prefab()
{
    SAFE_DELETE(m_p_prototype_game_object);
}

GameObject* Prefab::Instantiate() const
{
	return m_p_prototype_game_object->Clone();
}

bool Prefab::SaveToFile(const std::string& prefab_path)
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, prefab_path.c_str(), "wb"); //파일 쓰기

	if (p_file != nullptr)
	{
		p_save_game_object_func(m_p_prototype_game_object, p_file);

		fclose(p_file);

		return true;
	}

	else
		return false;
}

bool Prefab::LoadFromFile(const std::string& prefab_path)
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, prefab_path.c_str(), "rb"); //파일 읽기

	if (p_file != nullptr)
	{
		m_p_prototype_game_object = p_load_game_object_func(p_file);

		fclose(p_file);

		return true;
	}

	else
		return false;
}