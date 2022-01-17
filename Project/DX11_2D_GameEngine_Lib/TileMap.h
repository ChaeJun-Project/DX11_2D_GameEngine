#pragma once
#include "IComponent.h"

class Mesh;
class Material;
class Texture;

class StructuredBuffer;

//각 타일의 정보
struct TileInfo
{
	int tile_atlas_index;	//타일 아틀라스 텍스처 인덱스
    int tile_index;      	//타일 아틀라스 텍스처의 타일 인덱스
    Vector2 left_top;		//월드 공간상의 왼쪽 상단 좌표
	Vector2 right_bottom;	//월드 공간상의 오른쪽 하단 좌표
};

class TileMap : public IComponent
{
public:
	TileMap();
	explicit TileMap(const TileMap& origin);
	~TileMap();

	void Start() override;
	void FinalUpdate() override;
	void Render();

private:
    void CalcGridCoord();
	void BindPipeline();
	void DrawGrid();

public:
    //Atlas Texture
	const std::shared_ptr<Texture>& GetTileAtlasTexture(const UINT& tile_atlas_texture_index);
	void SetTileAtlasTexture(const std::shared_ptr<Texture>& p_tile_atlas_texture);
	
	const UINT GetUseTileAtlasTextureCount() { return m_tile_atlas_texture_count; }

	//Tile Count
	const Vector2 GetTileCount()
	{
		return Vector2(static_cast<float>(m_tile_count_x), static_cast<float>(m_tile_count_y));
	}
	void SetTileCount(const UINT& tile_count_x, const UINT& tile_count_y);

	//Current Tile Index
	const UINT GetCurrentTileIndex() { return m_current_tile_index; }
	void SetCurrentTileIndex(const UINT& current_tile_index) { m_current_tile_index = current_tile_index; }

	//Per Tile Size
	const Vector2 GetTileSize() { return m_tile_size; }
	void SetTileSize(const Vector2& tile_size) { m_tile_size = tile_size; }

	//Grid
	const bool& GetIsDrawGrid() { return m_is_draw_grid; }
	void SetIsDrawGrid(const bool& is_draw_grid) { m_is_draw_grid = is_draw_grid; }

private:
	void CalcCurrentPickRect(const Vector2& current_screen_pos);

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(TileMap);

private:
	//Tile Atlas Texture
	std::shared_ptr<Texture> m_p_current_tile_atlas_texture = nullptr;
    UINT m_tile_atlas_texture_count = 0;
	int m_current_tile_atlas_texture_index = -1;

	//Tile
	//타일 맵에 존재하는 모든 타일의 정보를 가지고 있을 구조적 버퍼
	std::shared_ptr<StructuredBuffer> m_p_tile_map_buffer = nullptr;
	//현재 선택된 타일 아틀라스 텍스처의 타일의 인덱스
	int m_current_tile_index = -1; 
	//사용자가 생성한 맵 타일에 대한 정보
	std::vector<TileInfo> m_tile_info_vector;

	UINT m_tile_count = 0;   //타일의 총 개수
	UINT m_tile_count_x = 0; //타일 맵의 한 행의 열의 개수
	UINT m_tile_count_y = 0; //타일 맵의 행의 개수
	Vector2 m_tile_size = Vector2(0.0f, 0.0f); //각 타일의 크기

	std::shared_ptr<Material> m_p_material = nullptr;
	std::shared_ptr<Mesh> m_p_mesh = nullptr;

	//Grid
	bool m_is_draw_grid = true;
	std::shared_ptr<Material> m_p_grid_material = nullptr;
	std::shared_ptr<Mesh> m_p_grid_mesh = nullptr;
	std::vector<Vector2> m_grid_left_top_vector;
};

