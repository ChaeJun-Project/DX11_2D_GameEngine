#pragma once
#include "IResource.h"

class GameObject;

class Prefab : public IResource
{
public:
	Prefab(GameObject* p_prototype_game_object);
	explicit Prefab(const Prefab& origin) = default;
	~Prefab();

public:
	GameObject* Instantiate();

public:
	bool SaveToFile(const std::string& prefab_path) override;
	bool LoadFromFile(const std::string& prefab_path) override;

public:
	CLONE(Prefab);

private:
    GameObject* m_p_prototype_game_object = nullptr;
};

