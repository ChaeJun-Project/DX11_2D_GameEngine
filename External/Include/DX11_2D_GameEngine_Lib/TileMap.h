#pragma once

#include "IResource.h"

class StructuredBuffer;

//�� Ÿ���� ����
struct TileData
{
	int tile_atlas_texture_index;	//Ÿ�� ��Ʋ�� �ؽ�ó �ε���
	int tile_index;      			//Ÿ�� ��Ʋ�� �ؽ�ó�� Ÿ�� �ε���
	Vector2 left_top;				//���� �������� ���� ��� ��ǥ
	Vector2 right_bottom;			//���� �������� ������ �ϴ� ��ǥ
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
	void SetTileCount(const UINT& tile_count_x, const UINT& tile_count_y, const std::vector<Vector2>& grid_left_top_vector); //���� �������� ��츸 ���
	void SetTileMapCoord(const UINT& tile_count_x, const UINT& tile_count_y, const std::vector<Vector2>& grid_left_top_vector); //Load ���� ��츸 ���
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
	UINT m_tile_count = 0;   //Ÿ���� �� ����
	Vector2 m_tile_size = Vector2(0.0f, 0.0f); //�� Ÿ���� ũ��
	UINT m_tile_count_x = 0; //Ÿ�� ���� �� ���� ���� ����
	UINT m_tile_count_y = 0; //Ÿ�� ���� ���� ����

	//Tile Data
	std::vector<TileData> m_tile_data_vector; //����ڰ� ������ �� Ÿ�Ͽ� ���� ����

	//TileMap Buffer
	//Ÿ�� �ʿ� �����ϴ� ��� Ÿ���� ������ ������ ���� ������ ����
	//Tile Data�� �� ���� Shader�� �����ϱ� ���� ���
	std::unique_ptr<StructuredBuffer> m_p_tile_map_buffer = nullptr;

	friend class TileMapRenderer;
};

