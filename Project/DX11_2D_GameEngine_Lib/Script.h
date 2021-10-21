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

protected:
    virtual void OnCollisionEnter(GameObject* other_game_object) {}
    virtual void OnCollisionExit(GameObject* other_game_object) {}
    virtual void OnCollision(GameObject* other_game_object) {}
 
};

