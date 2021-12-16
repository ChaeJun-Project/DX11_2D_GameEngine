#pragma once
#include "GUI\IGUI.h"

class GUI_StyleSelector;

class GUI_MenuBar : public IGUI
{
public:
    GUI_MenuBar(const std::string& menubar_title);
    ~GUI_MenuBar();

public:
    void Initialize() override;
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
     //Demo
     bool m_is_show_demo = false;

     //Style Selector
     bool m_is_show_style = false;
     GUI_StyleSelector* m_p_gui_style_selector = nullptr;
};

