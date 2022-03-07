#pragma once
#include "../GUI_Component.h"

class GUI_RigidBody2D final : public GUI_Component
{
public:
    GUI_RigidBody2D(const std::string& rigidbody2D_gui_name);
    ~GUI_RigidBody2D() = default;

public:
    void Render() override;
};

