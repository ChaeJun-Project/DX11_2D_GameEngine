#pragma once
#include "IComponent.h"

class Script : public IComponent
{
public:
    Script();
    ~Script() = default;

    virtual void Initialize() = 0;

    void Update() override;
    virtual void FinalUpdate() final;

};

