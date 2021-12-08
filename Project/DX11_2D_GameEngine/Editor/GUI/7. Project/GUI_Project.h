#pragma once
#include "GUI/IGUI.h"

class GUI_Project final : public IGUI
{
public:
    GUI_Project(const std::string& project_title);
    ~GUI_Project();

public:
    void Update() override;
    void Render() override;
    
};

