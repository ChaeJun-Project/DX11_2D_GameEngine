#pragma once
#include "IComponent.h"

class GameObject;

class Script : public IComponent
{
public:
    Script();
    ~Script() = default;

    virtual void Initialize() = 0;

    void Update() override;
    virtual void FinalUpdate() final;

public:
    void CreateGameObject(GameObject* p_new_game_object, const int& layer_index);
    void CreateGameObject(GameObject* p_new_game_object, const Vector3& position, const int& layer_index);
    void DeleteGameObject(GameObject* p_delete_game_object);

protected:
    virtual void OnCollisionEnter(GameObject* other_game_object) {}
    virtual void OnCollisionExit(GameObject* other_game_object) {}
    virtual void OnCollision(GameObject* other_game_object) {}
 
};

