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
    std::string m_tile_map_name;

    GUI_Palette* m_p_gui_palette = nullptr;
    GameObject* m_p_current_game_object = nullptr;
};

