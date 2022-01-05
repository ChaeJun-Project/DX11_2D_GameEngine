#include "stdafx.h"
#include "GUI_TileMap.h"

#include "Helper/IconProvider.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include "MapTool/GUI_MapTool.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/TileMap.h>

GUI_TileMap::GUI_TileMap(const std::string& tilemap_gui_name)
	:GUI_Component(tilemap_gui_name)
{
	m_p_gui_map_tool = new GUI_MapTool;
}

GUI_TileMap::~GUI_TileMap()
{
	SAFE_DELETE(m_p_gui_map_tool);

	m_p_current_game_object = nullptr;
}

void GUI_TileMap::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::TileMap, IconType::Component_TileMap))
	{
	    //Tiling Count
		static int tile_count_row = 0;
		static int tile_count_column = 0;

		//Tile Size
		static Vector2 tile_size = Vector2::Zero;

		if (m_p_current_game_object != nullptr && m_p_current_game_object != m_select_game_object)
		{
			auto tile_map = m_select_game_object->GetComponent<TileMap>();

			//Tiling Count
			auto tile_count = tile_map->GetTileCount();
			tile_count_row = static_cast<int>(tile_count.x);
			tile_count_column = static_cast<int>(tile_count.y);

			//Tile Size
			tile_size = tile_map->GetTileSize();
		}

		m_p_current_game_object = m_select_game_object;
		
		auto tile_map = m_p_current_game_object->GetComponent<TileMap>();

		//Map Tool
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 110.0f);
		if (ImGui::Button("Map Tool", ImVec2(110.0f, 0.0f)))
		{
			if (SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop)
			{
				m_p_gui_map_tool->m_is_active = true;
				m_p_gui_map_tool->m_p_current_tile_map = tile_map;
			}
		}

		//Use Tile Texture Count
		int use_count = static_cast<int>(tile_map->GetUseTileAtlasTextureCount());
		ShowInt("TileMap", "Tile Texture Count", use_count, 150.0f, ImGuiInputTextFlags_ReadOnly);

		//Tiling Count
		ShowInt2("Tiling", tile_count_row, tile_count_column, 70.0f, 80.0f);

		//Tile Size
		ShowFloat2("Tile Size", tile_size, 70.0f, 80.0f);

		//Editor 상태가 Play or Pause인 경우
		if (SceneManager::GetInstance()->GetEditorState() != EditorState::EditorState_Stop)
		{
			//Tiling Count
			auto tile_count = tile_map->GetTileCount();
			tile_count_row = static_cast<int>(tile_count.x);
			tile_count_column = static_cast<int>(tile_count.y);

			//Tile Size
			tile_size = tile_map->GetTileSize();
		}

		//Draw Grid Check
		bool is_active = tile_map->GetIsDrawGrid();
		if (ImGui::Checkbox("Draw Grid", &is_active))
		{
			if (SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop)
			{
				tile_map->SetIsDrawGrid(is_active);
			}
		}

		if (ImGui::Button("Apply", ImVec2(110.0f, 0.0f)))
		{
			if (SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop)
			{
				tile_map->SetTileCount(static_cast<UINT>(tile_count_row), static_cast<UINT>(tile_count_column));
				tile_map->SetTileSize(tile_size);
			}
		}

		DrawComponentEnd();
	}

	if (m_p_gui_map_tool->m_is_active)
	{
		m_p_gui_map_tool->Render();
	}
}
