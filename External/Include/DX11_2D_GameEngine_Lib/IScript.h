#pragma once
#include "IComponent.h"

class IScript : public IComponent
{
public:
    IScript(GameObject* p_game_object);
    ~IScript() = default;

    void Update() override;
    virtual void FinalUpdate() final;
};

