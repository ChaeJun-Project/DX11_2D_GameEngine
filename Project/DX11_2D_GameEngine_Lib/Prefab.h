#pragma once
#include "IResource.h"

class GameObject;

class Prefab : public IResource
{
public:
	Prefab(GameObject* p_prototype_game_object);
	~Prefab();

public:
	GameObject* Instantiate();

public:
	void LoadFromFile(const std::string& texture_path) override;
	void SaveFile(const std::string& texture_path) override;

private:
    GameObject* m_p_prototype_game_object = nullptr;
};

