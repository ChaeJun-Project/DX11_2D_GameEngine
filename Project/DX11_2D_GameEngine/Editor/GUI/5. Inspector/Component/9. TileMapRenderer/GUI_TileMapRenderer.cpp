#include "stdafx.h"
#include "GUI_TileMapRenderer.h"

#include "Helper/IconProvider.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include "Palette/GUI_Palette.h"

#include <DX11_2D_GameEngine_Lib/TileMap.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/TileMapRenderer.h>

GUI_TileMapRenderer::GUI_TileMapRenderer(const std::string& tilemap_gui_name)
	:GUI_Component(tilemap_gui_name)
{
	m_p_gui_palette = new GUI_Palette;
}

GUI_TileMapRenderer::~GUI_TileMapRenderer()
{
	SAFE_DELETE(m_p_gui_palette);

	m_p_current_game_object = nullptr;
}

void GUI_TileMapRenderer::Render()
{
	TileMapRenderer* tile_map_renderer = nullptr;

	//Tiling Count
	static int tile_count_row = 0;
	static int tile_count_column = 0;

	//Tile Size
	static Vector2 tile_size = Vector2::Zero;

	if (m_p_current_game_object != nullptr && m_p_current_game_object != m_select_game_object)
	{
		tile_map_renderer = m_select_game_object->GetComponent<TileMapRenderer>();

		//Tiling Count
		auto tile_count = tile_map_renderer->GetTileCount();
		tile_count_row = static_cast<int>(tile_count.x);
		tile_count_column = static_cast<int>(tile_count.y);

		//Tile Size
		tile_size = tile_map_renderer->GetTileSize();
	}

	m_p_current_game_object = m_select_game_object;

	tile_map_renderer = m_p_current_game_object->GetComponent<TileMapRenderer>();
	auto tile_map = tile_map_renderer->GetTileMap();
	if (tile_map != nullptr)
	{
		//Tiling Count
		auto tile_count = tile_map_renderer->GetTileCount();
		tile_count_row = static_cast<int>(tile_count.x);
		tile_count_column = static_cast<int>(tile_count.y);

		//Tile Size
		tile_size = tile_map_renderer->GetTileSize();
	}

	if (tile_map_renderer == nullptr)
		return;

	auto is_active = tile_map_renderer->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::TileMapRenderer, is_active, IconType::Component_TileMapRenderer))
	{
		auto tile_map_renderer = m_p_current_game_object->GetComponent<TileMapRenderer>();
		if (tile_map_renderer == nullptr)
			return;

		tile_map_renderer->SetIsActive(is_active);

		//Palette
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 110.0f);
		if (ImGui::Button("Palette", ImVec2(110.0f, 0.0f)))
		{
			if (CAN_EDIT)
			{
				m_p_gui_palette->m_is_active = true;
				m_p_gui_palette->m_p_current_tile_map = tile_map_renderer;

				tile_map_renderer->SetIsActivePalette(m_p_gui_palette->m_is_active);
			}
		}

		//Show Save TileMap Button
		if (ImGui::Button("Save TileMap", ImVec2(100.0f, 0.0f)))
		{
			if (CAN_EDIT)
			{
				if (tile_map != nullptr)
					FileFunction::SaveFile(TILEMAP_PATH, tile_map->GetResourceName(), FileType::TileMap);
			}
		}
		ImGui::SameLine();

		//Show Load TileMap Button
		if (ImGui::Button("Load TileMap", ImVec2(100.0f, 0.0f)))
		{
			if (CAN_EDIT)
			{

			}
			//std::string animation2D_path = FileFunction::LoadFile(ANIMATION_PATH, FileType::Animation);
		}

		//TileMap
		ImGui::Text("TileMap");
		ImGui::SameLine(100.0f);

		if(tile_map != nullptr)
			m_tile_map_name = tile_map->GetResourceName();

		ImGui::PushItemWidth(150.0f);
		if (ImGui::InputText("##TileMap Name", &m_tile_map_name, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (m_tile_map_name.size() && tile_map == nullptr)
			{
				tile_map_renderer->CreateTileMap(m_tile_map_name);
			}
		}
		ImGui::PopItemWidth();

		//Use Tile Texture Count
		int use_count = static_cast<int>(tile_map_renderer->GetUsedTileAtlasTextureCount());
		ShowInt("TileMapRenderer", "Used Texture Count", use_count, 150.0f, 0.0f, ImGuiInputTextFlags_ReadOnly);

		//Tiling Count
		ShowInt2("Tiling", tile_count_row, tile_count_column, 70.0f, 80.0f);

		//Tile Size
		ShowFloat2("Tile Size", tile_size, 70.0f, 80.0f);

		////Editor 상태가 Play or Pause인 경우
		//if (CAN_EDIT)
		//{
		//	//Tiling Count
		//	auto tile_count = tile_map_renderer->GetTileCount();
		//	tile_count_row = static_cast<int>(tile_count.x);
		//	tile_count_column = static_cast<int>(tile_count.y);

		//	//Tile Size
		//	tile_size = tile_map_renderer->GetTileSize();
		//}

		//Draw Grid Check
		bool is_active = tile_map_renderer->GetIsDrawGrid();
		if (ImGui::Checkbox("Draw Grid", &is_active))
		{
			if (CAN_EDIT)
			{
				tile_map_renderer->SetIsDrawGrid(is_active);
			}
		}

		if (ImGui::Button("Apply", ImVec2(110.0f, 0.0f)))
		{
			if (CAN_EDIT)
			{
				tile_map_renderer->SetTileCount(static_cast<UINT>(tile_count_row), static_cast<UINT>(tile_count_column));
				tile_map_renderer->SetTileSize(tile_size);
			}
		}

		DrawComponentEnd();

		if (m_p_gui_palette->m_is_active)
		{
			m_p_gui_palette->Render();
		}

		else
			tile_map_renderer->SetIsActivePalette(m_p_gui_palette->m_is_active);
	}
}
