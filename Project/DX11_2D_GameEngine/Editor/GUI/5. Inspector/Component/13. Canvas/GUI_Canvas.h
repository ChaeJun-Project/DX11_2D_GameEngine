#pragma once
#include "../GUI_Component.h"

class GUI_Canvas final : public GUI_Component
{
public:
    GUI_Canvas(const std::string& canvas_gui_name);
    ~GUI_Canvas() = default;

public:
    void Render() override;
};

