#include "stdafx.h"
#include "TileMapRenderer.h"

#include "StructuredBuffer.h"

#include "Mesh.h"
#include "Material.h"
#include "TileMap.h"
#include "Texture.h"

#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

TileMapRenderer::TileMapRenderer()
	:IComponent(ComponentType::TileMapRenderer)
{
	auto resource_manager = ResourceManager::GetInstance();

	m_p_mesh = resource_manager->GetResource<Mesh>("Rectangle_Mesh");
	auto clone_material = resource_manager->GetResource<Material>("TileMapRenderer_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(clone_material);

	m_p_grid_mesh = std::make_shared<Mesh>("Grid_Mesh");
	m_p_grid_mesh->Create(MeshType::Grid, 1, 1);
	clone_material = resource_manager->GetResource<Material>("Grid_Material")->Clone();
	m_p_grid_material = std::shared_ptr<Material>(clone_material);
}

TileMapRenderer::TileMapRenderer(const TileMapRenderer& origin)
	: IComponent(origin.GetComponentType())
{
}

TileMapRenderer::~TileMapRenderer()
{
	//Delete Tile Atlas Texture
	m_p_current_tile_atlas_texture.reset();

	//Delete TileMap
	m_p_tile_map.reset();

	//Delete Tile Render
	m_p_mesh.reset();
	m_p_material.reset();

	//Delete Grid Render
	m_p_grid_mesh.reset();
	m_p_grid_material.reset();
	m_grid_left_top_vector.clear();
}

void TileMapRenderer::FinalUpdate()
{
	if (m_p_tile_map == nullptr)
		return;

	auto render_manager = RenderManager::GetInstance();
	auto screen_offset = render_manager->GetScreenOffset();
	auto editor_camera = render_manager->GetEditorCamera();

	//Palette가 켜진 상태 & 편집 상태 & 마우스 왼쪽 클릭을 했을 때 
	if (m_is_active_palette && (SceneManager::GetInstance()->GetEditorState() == EditorState_Stop) && MOUSE_BUTTON_DOWN(KeyCode::CLICK_LEFT))
	{
		auto mouse_world_position = editor_camera->Picking();

		CalcCurrentPickRect(Vector2(mouse_world_position.x, mouse_world_position.y));
	}

	m_p_tile_map->FinalUpdate();
}

void TileMapRenderer::Render()
{
	if (m_p_tile_map == nullptr || m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

	CalcGridCoord();
	BindPipeline();
	m_p_mesh->Render();
	DrawGrid();
}

void TileMapRenderer::CalcGridCoord()
{
	auto tile_count = m_p_tile_map->m_tile_count;
	auto tile_size = m_p_tile_map->m_tile_size;
	auto tile_count_x = m_p_tile_map->m_tile_count_x;
	auto tile_count_y = m_p_tile_map->m_tile_count_y;

	auto transform = m_p_owner_game_object->GetComponent<Transform>();
	if (tile_size != Vector2::Zero && tile_count != 0)
	{
		transform->SetMeshScale
		(
			static_cast<UINT>(tile_count_x * tile_size.x),
			static_cast<UINT>(tile_count_y * tile_size.y)
		);

		auto world_matrix = transform->GetWorldMatrix();

		for (UINT i = 0; i < tile_count; ++i)
		{
			Vector3 left_top = Vector3(m_grid_left_top_vector[i].x, m_grid_left_top_vector[i].y, 0.0f);

			left_top = left_top * world_matrix;

			m_p_tile_map->m_tile_data_vector[i].left_top.x = left_top.x;
			m_p_tile_map->m_tile_data_vector[i].left_top.y = left_top.y;

			Vector3 tile_size_vec3 = Vector3(m_p_tile_map->m_tile_size.x, m_p_tile_map->m_tile_size.y, 0.0f);

			tile_size_vec3 = tile_size_vec3 * transform->GetLocalScale();

			m_p_tile_map->m_tile_data_vector[i].right_bottom = Vector2
			(
				m_p_tile_map->m_tile_data_vector[i].left_top.x + tile_size_vec3.x,
				m_p_tile_map->m_tile_data_vector[i].left_top.y - tile_size_vec3.y
			);
		}
	}
	transform->UpdateConstantBuffer();
}

void TileMapRenderer::BindPipeline()
{
	//Set Tile Count X
	m_p_material->SetConstantBufferData(Material_Parameter::INT_0, &(m_p_tile_map->m_tile_count_x));

	//Set Tile Count Y
	m_p_material->SetConstantBufferData(Material_Parameter::INT_1, &(m_p_tile_map->m_tile_count_y));

	m_p_material->BindPipeline();

	m_p_tile_map->BindPipeline();
}

void TileMapRenderer::DrawGrid()
{
	if (m_is_draw_grid && (SceneManager::GetInstance()->GetEditorState() == EditorState_Stop))
	{
		m_p_grid_material->BindPipeline();
		m_p_grid_mesh->Render();
	}
}

void TileMapRenderer::CreateTileMap(const std::string& tile_map_name)
{
	auto p_new_tile_map = ResourceManager::GetInstance()->CreateTileMap(tile_map_name);


	m_p_tile_map = p_new_tile_map;
}

const std::shared_ptr<Texture>& TileMapRenderer::GetTileAtlasTexture(const UINT& tile_atlas_texture_index)
{
	if (m_p_material)
	{
		if (!m_p_material->GetTextureVector().empty() && m_p_material->GetTextureVector()[tile_atlas_texture_index])
			return m_p_material->GetTextureVector()[tile_atlas_texture_index];

		else
			return nullptr;
	}

	else
		return nullptr;
}

void TileMapRenderer::SetTileAtlasTexture(const std::shared_ptr<Texture>& p_tile_atlas_texture)
{
	if (m_p_tile_map && m_p_material && m_p_tile_map->m_used_tile_atlas_texture_count != 8)
	{
	    auto used_texture_count = m_p_tile_map->m_used_tile_atlas_texture_count;

		//현재 타일맵에 사용될 텍스처 저장
		m_p_current_tile_atlas_texture = p_tile_atlas_texture;
		m_current_tile_atlas_texture_index = static_cast<int>(used_texture_count);

		//이미 등록된 텍스처인지 확인
		const auto& texture_vector = m_p_tile_map->m_used_tile_atlas_texture_vector;

		for (UINT i = 0; i < static_cast<UINT>(texture_vector.size()); ++i)
		{
			if (texture_vector[i] != nullptr && texture_vector[i]->GetResourceName() == p_tile_atlas_texture->GetResourceName())
			{
				m_current_tile_atlas_texture_index = static_cast<int>(i);
				return;
			}
		}

		m_p_tile_map->m_used_tile_atlas_texture_vector[used_texture_count] = p_tile_atlas_texture;

		//Texture
		UINT texture_offset = static_cast<UINT>(Material_Parameter::TEX_0) + used_texture_count;
		m_p_material->SetConstantBufferData(static_cast<Material_Parameter>(texture_offset), nullptr, m_p_tile_map->m_used_tile_atlas_texture_vector[used_texture_count]);

		//Texture Info
		//x: Texture Size X, y: Texture Size Y (Texture Size)
		//z: Texture_U , w: Texture_V (Per Tile UV Size In Texture)
		Vector4 texture_info = Vector4::Zero;

		//Set Atlas Texture Resolution
		texture_info.x = static_cast<float>(p_tile_atlas_texture->GetWidth());
		texture_info.y = static_cast<float>(p_tile_atlas_texture->GetHeight());

		//Set Atlas Textrue UV
		texture_info.z = m_p_tile_map->m_tile_size.x / texture_info.x;
		texture_info.w = m_p_tile_map->m_tile_size.y / texture_info.y;

		UINT texture_info_offset = static_cast<UINT>(Material_Parameter::VEC4_0) + used_texture_count;

		m_p_material->SetConstantBufferData(static_cast<Material_Parameter>(texture_info_offset), &texture_info);

		++m_p_tile_map->m_used_tile_atlas_texture_count;
	}
}

const UINT TileMapRenderer::GetUsedTileAtlasTextureCount()
{
	if (m_p_tile_map == nullptr)
		return 0;

	return m_p_tile_map->m_used_tile_atlas_texture_count;
}

const Vector2 TileMapRenderer::GetTileCount()
{
	if (m_p_tile_map == nullptr)
		return Vector2::Zero;

	return 	Vector2(static_cast<float>(m_p_tile_map->m_tile_count_x), static_cast<float>(m_p_tile_map->m_tile_count_y));
}

void TileMapRenderer::SetTileCount(const UINT& tile_count_x, const UINT& tile_count_y)
{
	if (m_p_tile_map == nullptr)
		return;

	CreateGrid(tile_count_x, tile_count_y);

	m_p_tile_map->SetTileCount(tile_count_x, tile_count_y, m_grid_left_top_vector);
}

const Vector2 TileMapRenderer::GetTileSize()
{
	if (m_p_tile_map == nullptr)
		return Vector2::Zero;

	return m_p_tile_map->m_tile_size;
}

void TileMapRenderer::SetTileSize(const Vector2& tile_size)
{
	if (m_p_tile_map == nullptr)
		return;

	m_p_tile_map->m_tile_size = tile_size;
}

void TileMapRenderer::CreateGrid(const UINT& tile_count_x, const UINT& tile_count_y)
{
	if (!m_grid_left_top_vector.empty())
	{
		m_grid_left_top_vector.clear();
	}

	m_p_grid_mesh->Create(MeshType::Grid, tile_count_x, tile_count_y);

	m_grid_left_top_vector = m_p_grid_mesh->GetGridLeftTopVector();
}

void TileMapRenderer::CalcCurrentPickRect(const Vector2& current_screen_pos)
{
	for (int i = 0; i < static_cast<int>(m_p_tile_map->m_tile_data_vector.size()); ++i)
	{
		if (RenderManager::GetInstance()->CheckMouseWorldPositionInRect(
			current_screen_pos,
			m_p_tile_map->m_tile_data_vector[i].left_top,
			m_p_tile_map->m_tile_data_vector[i].right_bottom))
		{
			m_p_tile_map->m_tile_data_vector[i].tile_atlas_texture_index = m_current_tile_atlas_texture_index;
			m_p_tile_map->m_tile_data_vector[i].tile_index = m_current_tile_index;
		}
	}
}

void TileMapRenderer::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	auto resource_manager = ResourceManager::GetInstance();

	//TileMap
	fprintf(p_file, "■ TileMap\n");
	resource_manager->SaveResource<TileMap>(m_p_tile_map, p_file);
	fprintf(p_file, "[Material]\n");
	resource_manager->SaveResource<Material>(m_p_material, p_file);
	fprintf(p_file, "[Mesh]\n");
	resource_manager->SaveResource<Mesh>(m_p_mesh, p_file);

	//Grid
	fprintf(p_file, "■ Grid\n");
	fprintf(p_file, "[Draw]\n");
	fprintf(p_file, "%d\n", m_is_draw_grid);
	fprintf(p_file, "[Material]\n");
	resource_manager->SaveResource<Material>(m_p_grid_material, p_file);
	fprintf(p_file, "[Mesh]\n");
	resource_manager->SaveResource<Mesh>(m_p_grid_mesh, p_file);
}

void TileMapRenderer::LoadFromScene(FILE* p_file)
{
	auto resource_manager = ResourceManager::GetInstance();

	char char_buffer[256] = { 0 };

	//TileMap
	FILE_MANAGER->FScanf(char_buffer, p_file); //■ TileMap
	resource_manager->LoadResource<TileMap>(m_p_tile_map, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Material]
	resource_manager->LoadResource<Material>(m_p_material, p_file);
	for (UINT i = 0; i < m_p_tile_map->m_used_tile_atlas_texture_count; ++i)
	{
		LoadSetTileAtlasTexture(m_p_tile_map->m_used_tile_atlas_texture_vector[i], i);
	}
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Mesh]
	resource_manager->LoadResource<Mesh>(m_p_mesh, p_file);

	//Grid
	FILE_MANAGER->FScanf(char_buffer, p_file); //■ Grid
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Draw]
	fscanf_s(p_file, "%d\n", &m_is_draw_grid);
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Material]
	resource_manager->LoadResource<Material>(m_p_grid_material, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Mesh]
	resource_manager->LoadResource<Mesh>(m_p_grid_mesh, p_file);

	LoadSetTileCount(m_p_tile_map->m_tile_count_x, m_p_tile_map->m_tile_count_y);
}

void TileMapRenderer::LoadSetTileAtlasTexture(const std::shared_ptr<Texture>& p_tile_atlas_texture, const UINT& offset)
{
	UINT texture_offset = static_cast<UINT>(Material_Parameter::TEX_0) + offset;
	m_p_material->SetConstantBufferData(static_cast<Material_Parameter>(texture_offset), nullptr, p_tile_atlas_texture);

	//Texture Info
	//x: Texture Size X, y: Texture Size Y (Texture Size)
	//z: Texture_U , w: Texture_V (Per Tile UV Size In Texture)
	Vector4 texture_info = Vector4::Zero;

	//Set Atlas Texture Resolution
	texture_info.x = static_cast<float>(p_tile_atlas_texture->GetWidth());
	texture_info.y = static_cast<float>(p_tile_atlas_texture->GetHeight());

	//Set Atlas Textrue UV
	texture_info.z = m_p_tile_map->m_tile_size.x / texture_info.x;
	texture_info.w = m_p_tile_map->m_tile_size.y / texture_info.y;

	UINT texture_info_offset = static_cast<UINT>(Material_Parameter::VEC4_0) + offset;

	m_p_material->SetConstantBufferData(static_cast<Material_Parameter>(texture_info_offset), &texture_info);
}

void TileMapRenderer::LoadSetTileCount(const UINT& tile_count_x, const UINT& tile_count_y)
{
	if (m_p_tile_map == nullptr)
		return;

	CreateGrid(tile_count_x, tile_count_y);

	m_p_tile_map->SetTileMapCoord(tile_count_x, tile_count_y, m_grid_left_top_vector);

	EDITOR_LOG_INFO_F("Tile Size: %dx%d", tile_count_x, tile_count_y);
}
