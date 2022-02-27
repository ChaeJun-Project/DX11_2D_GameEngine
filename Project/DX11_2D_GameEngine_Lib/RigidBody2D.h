#pragma once

#include "IComponent.h"

//TODO
class RigidBody2D final : public IComponent
{
public:
    RigidBody2D();
    explicit RigidBody2D(const RigidBody2D& origin);
    ~RigidBody2D();

    void Initialize() override;
    void Start() override;
    void FinalUpdate() override;

private:
    void SaveToScene(FILE* p_file) override;
    void LoadFromScene(FILE* p_file) override;

private:
    CLONE(RigidBody2D);

private:
    float m_gravity_scale = 0.0f;
};

