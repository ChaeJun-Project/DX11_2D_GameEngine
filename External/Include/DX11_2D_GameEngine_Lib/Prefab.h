#pragma once
#include "IResource.h"

typedef std::function<void(GameObject*, FILE*)> SaveGameObjectFunc;
typedef std::function<GameObject* (FILE*)> LoadGameObjectFunc;

class GameObject;

class Prefab : public IResource
{
public:
	//<summary>
	//Client의 ClientSceneManager 클래스의 SaveGameObject, LoadGameObject 함수를 바인딩 하여 사용
	//</summary>
	static SaveGameObjectFunc p_save_game_object_func;
	static LoadGameObjectFunc p_load_game_object_func;

public:
	Prefab(const std::string& prefab_resource_name);
	Prefab(GameObject* p_prototype_game_object);
	~Prefab();

public:
	GameObject* Instantiate() const;

public:
	GameObject* GetGameObject() const { return m_p_prototype_game_object; }

public:
	bool SaveToFile(const std::string& prefab_path) override;
	bool LoadFromFile(const std::string& prefab_path) override;

private:
    GameObject* m_p_prototype_game_object = nullptr;
};

