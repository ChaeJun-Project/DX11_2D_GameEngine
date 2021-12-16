#pragma once
#include "GUI\IGUI.h"

class GUI_ToolBar : public IGUI
{
public:
    GUI_ToolBar(const std::string& toolbar_title);
    ~GUI_ToolBar() = default;

public:
    void Initialize() override;
    void Update() override;

    void Begin() override;
    void Render() override;
  
private:
};

