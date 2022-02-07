#pragma once

#include "IResource.h"

class StructuredBuffer;

//각 타일의 정보
struct TileData
{
	int tile_atlas_texture_index;	//타일 아틀라스 텍스처 인덱스
	int tile_index;      			//타일 아틀라스 텍스처의 타일 인덱스
	Vector2 left_top;				//월드 공간상의 왼쪽 상단 좌표
	Vector2 right_bottom;			//월드 공간상의 오른쪽 하단 좌표
};

class TileMap final : public IResource
{
public:
	TileMap(const std::string& tile_map_resource_name);
	explicit TileMap(const TileMap& origin);
	~TileMap();

	void FinalUpdate();
	void BindPipeline() override;

private:
	void SetTileCount(const UINT& tile_count_x, const UINT& tile_count_y, const std::vector<Vector2>& grid_left_top_vector); //최초 생성했을 경우만 사용
	void SetTileMapCoord(const UINT& tile_count_x, const UINT& tile_count_y, const std::vector<Vector2>& grid_left_top_vector); //Load 했을 경우만 사용
	void CreateTileMapBuffer();

public:
	bool SaveToFile(const std::string& tile_map_path) override;
	bool LoadFromFile(const std::string& tile_map_path) override;

public:
	CLONE(TileMap);

private:
	//Used Tile Atlas Texture
	std::vector<std::shared_ptr<Texture>> m_used_tile_atlas_texture_vector;
	UINT m_used_tile_atlas_texture_count = 0;

	//TileMap Info
	UINT m_tile_count = 0;   //타일의 총 개수
	Vector2 m_tile_size = Vector2(0.0f, 0.0f); //각 타일의 크기
	UINT m_tile_count_x = 0; //타일 맵의 한 행의 열의 개수
	UINT m_tile_count_y = 0; //타일 맵의 행의 개수

	//Tile Data
	std::vector<TileData> m_tile_data_vector; //사용자가 생성한 맵 타일에 대한 정보

	//TileMap Buffer
	//타일 맵에 존재하는 모든 타일의 정보를 가지고 있을 구조적 버퍼
	//Tile Data를 한 번에 Shader에 전달하기 위해 사용
	std::unique_ptr<StructuredBuffer> m_p_tile_map_buffer = nullptr;

	friend class TileMapRenderer;
};

