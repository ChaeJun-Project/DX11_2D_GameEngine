#pragma once
#include "IResource.h"

class GameObject;

class Prefab : public IResource
{
private:
	Prefab(const std::string& resource_name);
	Prefab(GameObject* p_prototype_game_object);

public:
	~Prefab();

	GameObject* Instantiate();


private:
    GameObject* m_p_prototype_game_object = nullptr;

	friend class ResourceManager;
};

