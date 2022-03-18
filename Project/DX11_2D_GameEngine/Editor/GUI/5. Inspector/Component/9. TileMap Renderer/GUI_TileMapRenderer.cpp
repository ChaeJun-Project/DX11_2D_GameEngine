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
}

void GUI_TileMapRenderer::Render()
{
	//TileMapRenderer
	auto p_tile_map_renderer = m_p_selected_game_object->GetComponent<TileMapRenderer>();
	if (p_tile_map_renderer == nullptr)
		return;

	auto is_active = p_tile_map_renderer->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::TileMapRenderer, is_active, IconType::Component_TileMapRenderer))
	{
		auto p_tile_map_renderer = m_p_selected_game_object->GetComponent<TileMapRenderer>();
		if (p_tile_map_renderer == nullptr)
			return;

		p_tile_map_renderer->SetIsActive(is_active);

		//TileMap
		auto p_tile_map = p_tile_map_renderer->GetTileMap();

		//Tiling Count
		auto tile_count_x = static_cast<int>(p_tile_map_renderer->GetTileCountX()); //타일의 열의 개수(X축 타일 개수)
		auto tile_count_y = static_cast<int>(p_tile_map_renderer->GetTileCountY()); //타일의 행의 개수(Y축 타일 개수)

		//Tile Size
		auto tile_size = p_tile_map_renderer->GetTileSize();

		//Palette
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 110.0f);
		if (ImGui::Button("Palette", ImVec2(110.0f, 0.0f)))
		{
			if (CAN_EDIT)
			{
				if (!m_p_gui_palette->m_is_active)
				{
					m_p_gui_palette->Initialize();
					m_p_gui_palette->m_is_active = true;
					m_p_gui_palette->m_p_current_tile_map = p_tile_map_renderer;

					p_tile_map_renderer->SetIsActivePalette(m_p_gui_palette->m_is_active);
				}
			}
		}

		//Save TileMap Button
		if (ImGui::Button("Save TileMap", ImVec2(100.0f, 0.0f)))
		{
			if (CAN_EDIT)
			{
				if (p_tile_map != nullptr)
					FileFunction::SaveFile(TILEMAP_PATH, p_tile_map->GetResourceName(), FileType::TileMap);
			}
		}
		ImGui::SameLine();

		//Load TileMap Button
		if (ImGui::Button("Load TileMap", ImVec2(100.0f, 0.0f)))
		{
			if (CAN_EDIT)
			{
				auto tile_map_path = FileFunction::LoadFile(TILEMAP_PATH, FileType::TileMap);

				if (!tile_map_path.empty())
				{
					auto p_tile_map = RESOURCE_MANAGER->LoadFromFile<TileMap>(tile_map_path);
					p_tile_map_renderer->SetTileMap(p_tile_map);
				}
			}
		}

		//TileMap
		ImGui::Text("TileMap");
		ImGui::SameLine(100.0f);

		std::string tile_map_name;
		if (p_tile_map != nullptr)
			tile_map_name = p_tile_map->GetResourceName();

		ImGui::PushItemWidth(150.0f);
		if (ImGui::InputText("##TileMap Name", &tile_map_name, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (tile_map_name.size())
			{
				p_tile_map_renderer->CreateTileMap(tile_map_name);
			}
		}
		ImGui::PopItemWidth();

		//Use Tile Texture Count
		int use_count = static_cast<int>(p_tile_map_renderer->GetUsedTileAtlasTextureCount());
		ShowInt("TileMapRenderer", "Used Texture Count", use_count, 150.0f, 0.0f, ImGuiInputTextFlags_ReadOnly);

		//Tiling Count
		ShowInt2("Tiling", tile_count_x, tile_count_y, 70.0f, 80.0f);

		//Tile Size
		ShowFloat2("Tile Size", tile_size, 70.0f, 80.0f);

		//Editor 상태가 Play or Pause인 경우
		//Update TileMap Property
		if (CAN_EDIT)
		{
			//Tiling Count
			p_tile_map_renderer->SetTileCountX(tile_count_x);
			p_tile_map_renderer->SetTileCountY(tile_count_y);

			//Tile Size
			p_tile_map_renderer->SetTileSizeX(tile_size.x);
			p_tile_map_renderer->SetTileSizeY(tile_size.y);
		}

		//Grid Check
		bool is_active = p_tile_map_renderer->GetIsDrawGrid();
		if (ImGui::Checkbox("Draw Grid", &is_active))
		{
			if (CAN_EDIT)
			{
				p_tile_map_renderer->SetIsDrawGrid(is_active);
			}
		}

		//TileMap Property Apply
		if (ImGui::Button("Apply", ImVec2(110.0f, 0.0f)))
		{
			if (CAN_EDIT)
			{
				p_tile_map_renderer->SetIsResizeTileMap(true);
				p_tile_map_renderer->CreateGrid(tile_count_x, tile_count_y);
				p_tile_map_renderer->CreateTileMapData();
			}
		}

		DrawComponentEnd();

		if (m_p_gui_palette->m_is_active)
		{
			m_p_gui_palette->Render();
			p_tile_map_renderer->EditTileMap();
		}

		else
			p_tile_map_renderer->SetIsActivePalette(m_p_gui_palette->m_is_active);
	}
}