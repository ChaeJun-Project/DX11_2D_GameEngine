#pragma once
#include "IComponent.h"

class Prefab;
class GameObject;

class Script : public IComponent
{
public:
	Script(const std::string& script_name);
	explicit Script(const Script& origin);
	virtual ~Script() = default;

	virtual void Start() {}
	void Update() override {}
	virtual void FinalUpdate() final {}

protected:
	//GameObject
	GameObject* Instantiate(std::shared_ptr<Prefab>& p_game_object_prefab, const Vector3& position, const UINT& layer_index, const bool& use_event = false);
	void Destroy(GameObject* p_delete_game_object);
	void CreateGameObject(GameObject* p_new_game_object, const UINT& layer_index);

public:
	//Collision
	virtual void OnCollisionEnter(GameObject* other_game_object) {}
	virtual void OnCollisionExit(GameObject* other_game_object) {}
	virtual void OnCollision(GameObject* other_game_object) {}

public:
	const std::string& GetScriptName() const { return m_script_name; }

public:
	virtual void SaveToScene(FILE* p_file);
	virtual void LoadFromScene(FILE* p_file);

public:
	virtual Script* Clone() = 0;

protected:
	const std::string m_script_name;
};

