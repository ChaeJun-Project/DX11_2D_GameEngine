#pragma once
#include "IComponent.h"

class Mesh;
class Material;
class Texture;
class TileMap;

class TileMapRenderer : public IComponent
{
public:
	TileMapRenderer();
	explicit TileMapRenderer(const TileMapRenderer& origin);
	~TileMapRenderer();

	void FinalUpdate() override;
	void Render();

private:
    void CalcGridCoord();
	void BindPipeline();
	void DrawGrid();

public:
    void SetIsActivePalette(const bool& is_active_palette) { m_is_active_palette = is_active_palette; }

	void CreateTileMap(const std::string& tile_map_name);
	
	const std::shared_ptr<TileMap>& GetTileMap() { SAFE_GET_POINTER(m_p_tile_map); }
	void SetTileMap(const std::shared_ptr<TileMap>& p_tile_map) { m_p_tile_map = p_tile_map; }

    //Atlas Texture
	const std::shared_ptr<Texture>& GetTileAtlasTexture(const UINT& tile_atlas_texture_index);
	void SetTileAtlasTexture(const std::shared_ptr<Texture>& p_tile_atlas_texture);
	
	const UINT GetUsedTileAtlasTextureCount();

	//Current Tile Index
	const UINT GetCurrentTileIndex() { return m_current_tile_index; }
	void SetCurrentTileIndex(const UINT& current_tile_index) { m_current_tile_index = current_tile_index; }

	//Tile Count
	const Vector2 GetTileCount();
	void SetTileCount(const UINT& tile_count_x, const UINT& tile_count_y);

	//Tile Size
	const Vector2 GetTileSize();
	void SetTileSize(const Vector2& tile_size);

	//Grid
	const bool& GetIsDrawGrid() { return m_is_draw_grid; }
	void SetIsDrawGrid(const bool& is_draw_grid) { m_is_draw_grid = is_draw_grid; }

private:
	void CreateGrid(const UINT& tile_count_x, const UINT& tile_count_y);
	void CalcCurrentPickRect(const Vector2& current_screen_pos);

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
    void LoadSetTileAtlasTexture(const std::shared_ptr<Texture>& p_tile_atlas_texture, const UINT& offset);
    void LoadSetTileCount(const UINT& tile_count_x, const UINT& tile_count_y);

public:
	CLONE(TileMapRenderer);

private:
    bool m_is_active_palette = false;

	//Tile Atlas Texture
	std::shared_ptr<Texture> m_p_current_tile_atlas_texture = nullptr;
	//Tile Atlas Texture의 Tile Index
	int m_current_tile_atlas_texture_index = -1;

	//TileMap
	std::shared_ptr<TileMap> m_p_tile_map = nullptr;
	
	//현재 선택된 타일 아틀라스 텍스처의 타일의 인덱스
	int m_current_tile_index = -1;
	std::shared_ptr<Material> m_p_material = nullptr;
	std::shared_ptr<Mesh> m_p_mesh = nullptr;

	//Grid
	bool m_is_draw_grid = true;
	std::shared_ptr<Material> m_p_grid_material = nullptr;
	std::shared_ptr<Mesh> m_p_grid_mesh = nullptr;
	std::vector<Vector2> m_grid_left_top_vector;
};

