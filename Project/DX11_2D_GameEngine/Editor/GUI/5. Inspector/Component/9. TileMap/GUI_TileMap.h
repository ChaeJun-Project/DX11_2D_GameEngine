#pragma once
#include "../GUI_Component.h"

class GUI_MapTool;
class GameObject;

class GUI_TileMap : public GUI_Component
{
public:
    GUI_TileMap(const std::string& tilemap_gui_name);
    ~GUI_TileMap();

public:
    void Render() override;

private:
    GUI_MapTool* m_p_gui_map_tool = nullptr;
    GameObject* m_p_current_game_object = nullptr;
};

