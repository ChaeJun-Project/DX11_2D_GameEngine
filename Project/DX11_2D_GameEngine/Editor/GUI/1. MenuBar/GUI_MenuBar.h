#pragma once
#include "GUI\IGUI.h"

class GUI_StyleSelector;
class GUI_SpriteEditor;

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
    void ShowRenameScene();

    void SaveCurrentScene();
    void LoadScene();

private:
     //=========================
     // File
     //=========================
     bool m_is_show_rename_scene = false;

     //=========================
     // Edit
     //=========================
     //Style Selector
     GUI_StyleSelector* m_p_gui_style_selector = nullptr;

     //SpriteAnimation
     GUI_SpriteEditor* m_p_gui_sprite_editor = nullptr;
};

