#pragma once
#include "../GUI_Component.h"

class GUI_ItemList;

class GUI_Canvas final : public GUI_Component
{
public:
    GUI_Canvas(const std::string& canvas_gui_name);
    ~GUI_Canvas();

public:
    void Render() override;

private:
    GUI_ItemList* m_p_camera_object_list = nullptr;
};

