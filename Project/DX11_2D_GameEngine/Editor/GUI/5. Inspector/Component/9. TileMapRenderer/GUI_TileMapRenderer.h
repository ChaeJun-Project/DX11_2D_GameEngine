#pragma once
#include "../GUI_Component.h"

class GUI_Palette;
class GameObject;

class GUI_TileMapRenderer : public GUI_Component
{
public:
    GUI_TileMapRenderer(const std::string& tilemap_gui_name);
    ~GUI_TileMapRenderer();

public:
    void Render() override;

private:
    GUI_Palette* m_p_gui_palette = nullptr;
};

