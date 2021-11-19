#pragma once
#include "GUI\IGUI.h"

class GUI_MenuBar : public IGUI
{
public:
    GUI_MenuBar(const std::string& menubar_title);
    ~GUI_MenuBar() = default;

public:
    void Update() override;

    void Begin() override {}
    void Render() override;
    void End() override {}

private:
     //=========================
     // File
     //=========================
     bool m_is_scene_load = false;
     bool m_is_scene_save = false;

     //=========================
     // Edit
     //=========================
     bool m_is_show_demo = false;
     bool m_is_show_style = false;
};

