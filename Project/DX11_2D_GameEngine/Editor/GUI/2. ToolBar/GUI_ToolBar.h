#pragma once
#include "GUI\IGUI.h"

class GUI_ToolBar : public IGUI
{
public:
    GUI_ToolBar(const std::string& toolbar_title);
    virtual ~GUI_ToolBar() = default;

public:
    virtual void Begin() override;
    virtual void Render() override;
    virtual void End() override;
};

