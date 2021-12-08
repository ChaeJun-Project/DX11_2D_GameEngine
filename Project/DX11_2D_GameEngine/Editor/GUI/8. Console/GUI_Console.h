#pragma once
#include "GUI/IGUI.h"

class GUI_Console final : public IGUI
{
public:
    GUI_Console(const std::string& console_title);
    ~GUI_Console();

public:
    void Update() override;
    void Render() override;

};


