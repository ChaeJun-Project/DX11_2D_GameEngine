#pragma once
#include "IComponent.h"

class Prefab;
class GameObject;

class Script : public IComponent
{
public:
	Script();
	~Script() = default;

	virtual void Initialize() = 0;

	void Update() override {}
	virtual void FinalUpdate() final {}

protected:
	void Instantiate(std::shared_ptr<Prefab>& p_game_object_prefab, const Vector3& position, const UINT& layer_index);
	void Destroy(GameObject* p_delete_game_object);

private:
	//GameObject
	void CreateGameObject(GameObject* p_new_game_object, const UINT& layer_index);

protected:
	//Collision
	virtual void OnCollisionEnter(GameObject* other_game_object) {}
	virtual void OnCollisionExit(GameObject* other_game_object) {}
	virtual void OnCollision(GameObject* other_game_object) {}
};

