#include "stdafx.h"
#include "TileMap.h"

#include "ResourceManager.h"

#include "StructuredBuffer.h"

TileMap::TileMap(const std::string& tile_map_resource_name)
	:IResource(ResourceType::TileMap, tile_map_resource_name)
{
	m_p_tile_map_buffer = std::make_unique<StructuredBuffer>();

	m_used_tile_atlas_texture_vector.resize(8);
}

TileMap::TileMap(const TileMap& origin)
	: IResource(origin.m_resource_type, origin.m_object_name)
{
}

TileMap::~TileMap()
{
	//Delete Used Tile Atlas Texture
	for (auto& used_tile_atlas_texture : m_used_tile_atlas_texture_vector)
	{
		if (used_tile_atlas_texture != nullptr)
			used_tile_atlas_texture.reset();
	}

	m_used_tile_atlas_texture_vector.clear();
	m_used_tile_atlas_texture_vector.shrink_to_fit();

	//Delete Tile Data
	m_tile_data_vector.clear();
	m_tile_data_vector.shrink_to_fit();

	//Delete TileMap Buffer
	m_p_tile_map_buffer.reset();
}

void TileMap::FinalUpdate()
{
	if (!m_tile_data_vector.empty())
		m_p_tile_map_buffer->SetStructuredBufferData(m_tile_data_vector.data(), sizeof(TileData) * m_tile_count);
}

void TileMap::BindPipeline()
{
	if (!m_tile_data_vector.empty())
	{
		//Set Tile Map Buffer
		m_p_tile_map_buffer->SetBufferBindStage(PipelineStage::PS);
		m_p_tile_map_buffer->SetBufferBindSlot(15);
		m_p_tile_map_buffer->BindPipeline();
	}
}

void TileMap::CreateTileData(const std::vector<Vector2>& grid_left_top_vector)
{
    //TileMap의 Tile Count가 설정되지 않은 경우
    if(m_tile_count_x <= 0 || m_tile_count_y <= 0)
	  return;

	//각 타일의 정보가 이미 저장되어 있다면
	if (!m_tile_data_vector.empty())
		m_tile_data_vector.clear(); //클리어

	m_tile_count = m_tile_count_x * m_tile_count_y;
	m_tile_data_vector.resize(m_tile_count);

	for (UINT i = 0; i < m_tile_count; ++i)
	{
		m_tile_data_vector[i].tile_atlas_texture_index = -1;
		m_tile_data_vector[i].tile_index = -1;
		m_tile_data_vector[i].left_top = grid_left_top_vector[i];
	}

	CreateTileMapBuffer();
}

void TileMap::SetTileMapCoord(const UINT& tile_count_x, const UINT& tile_count_y, const std::vector<Vector2>& grid_left_top_vector)
{
	for (UINT i = 0; i < m_tile_count; ++i)
	{
		m_tile_data_vector[i].left_top = grid_left_top_vector[i];
	}

	CreateTileMapBuffer();
}

void TileMap::CreateTileMapBuffer()
{
	//각 타일의 정보가 이미 저장되어 있다면
	if (!m_tile_data_vector.empty())
		m_p_tile_map_buffer->Create(sizeof(TileData), m_tile_count, SBufferType::Read_Only, true, m_tile_data_vector.data());
}

bool TileMap::SaveToFile(const std::string& tile_map_path)
{
	auto resource_manager = RESOURCE_MANAGER;

	FILE* p_file = nullptr;
	fopen_s(&p_file, tile_map_path.c_str(), "wb"); //파일 쓰기

	if (p_file != nullptr)
	{
		//TileMap Name
		fprintf(p_file, "[TileMap Name]\n");
		fprintf(p_file, "%s\n", m_object_name.c_str());

		//Used Atlas Texture List
		fprintf(p_file, "[Used Atlas Texture List]\n");
		fprintf(p_file, "[Count]\n");
		fprintf(p_file, "%d\n", m_used_tile_atlas_texture_count);
		for (UINT i = 0; i < m_used_tile_atlas_texture_vector.size(); ++i)
		{
			auto used_tile_atlas_texture = m_used_tile_atlas_texture_vector[i];
			resource_manager->SaveResource<Texture>(used_tile_atlas_texture, p_file);
		}

		//TileMap Info
		fprintf(p_file, "[Tile Count]\n");
		fprintf(p_file, "%d\n", m_tile_count);
		fprintf(p_file, "[Tile Size]\n");
		FILE_MANAGER->FPrintf_Vector2(m_tile_size, p_file);
		fprintf(p_file, "[Column Count]\n");
		fprintf(p_file, "%d\n", m_tile_count_x);
		fprintf(p_file, "[Row Count]\n");
		fprintf(p_file, "%d\n", m_tile_count_y);

		//Tile Data
		fprintf(p_file, "[Tile Data]\n");
		fprintf(p_file, "[Count]\n");
		fprintf(p_file, "%d\n", m_tile_data_vector.size());
		for (UINT i = 0; i < m_tile_data_vector.size(); ++i)
		{
			auto tile_data = m_tile_data_vector[i];

			fprintf(p_file, "%d ", tile_data.tile_atlas_texture_index);
			fprintf(p_file, "%d\n", tile_data.tile_index);
		}

		fclose(p_file);
		return true;
	}

	else
		return false;
}

bool TileMap::LoadFromFile(const std::string& tile_map_path)
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, tile_map_path.c_str(), "rb"); //파일 읽기

	if (p_file != nullptr)
	{
		char char_buffer[256] = {};

		//TileMap Name
		FILE_MANAGER->FScanf(char_buffer, p_file); //[TileMap Name]
		FILE_MANAGER->FScanf(char_buffer, p_file);
		m_object_name = std::string(char_buffer);

		//Used Atlas Texture List
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Used Atlas Texture List]
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Count]
		fscanf_s(p_file, "%d\n", &m_used_tile_atlas_texture_count);
		for (UINT i = 0; i < m_used_tile_atlas_texture_vector.size(); ++i)
		{
			std::shared_ptr<Texture> m_p_atlas_texture = nullptr;
			RESOURCE_MANAGER->LoadResource<Texture>(m_p_atlas_texture, p_file);

			m_used_tile_atlas_texture_vector[i] = m_p_atlas_texture;
		}

		//TileMap Info
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Tile Count]
		fscanf_s(p_file, "%d\n", &m_tile_count);
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Tile Size]
		FILE_MANAGER->FScanf_Vector2(m_tile_size, p_file);
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Column Count]
		fscanf_s(p_file, "%d\n", &m_tile_count_x);
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Row Count]
		fscanf_s(p_file, "%d\n", &m_tile_count_y);

		//Tile Data
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Tile Data]
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Count]
		UINT tile_data_count = 0;
		fscanf_s(p_file, "%d\n", &tile_data_count);

		m_tile_data_vector.resize(tile_data_count);
		for (UINT i = 0; i < tile_data_count; ++i)
		{
			TileData tile_data;
			ZeroMemory(&tile_data, sizeof(TileData));

			fscanf_s(p_file, "%d ", &tile_data.tile_atlas_texture_index);
			fscanf_s(p_file, "%d\n", &tile_data.tile_index);

			m_tile_data_vector[i] = tile_data;
		}

		CreateTileMapBuffer();

		fclose(p_file);

		return true;
	}

	else
		return false;
}