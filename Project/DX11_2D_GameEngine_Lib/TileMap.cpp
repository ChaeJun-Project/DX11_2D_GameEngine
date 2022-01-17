#include "stdafx.h"
#include "TileMap.h"

#include "SceneManager.h"
#include "RenderManager.h"

#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"

#include "StructuredBuffer.h"

#include "Transform.h"
#include "Camera.h"

TileMap::TileMap()
	:IComponent(ComponentType::TileMap)
{
	m_p_tile_map_buffer = std::make_shared<StructuredBuffer>();

	auto resource_manager = ResourceManager::GetInstance();

	m_p_mesh = resource_manager->GetResource<Mesh>("Rectangle_Mesh");
	auto clone_material = resource_manager->GetResource<Material>("TileMap_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(clone_material);

	m_p_grid_mesh = std::make_shared<Mesh>("Grid_Mesh");
	m_p_grid_mesh->Create(MeshType::Grid, 1, 1);
	clone_material = resource_manager->GetResource<Material>("Grid_Material")->Clone();
	m_p_grid_material = std::shared_ptr<Material>(clone_material);
}

TileMap::TileMap(const TileMap& origin)
	: IComponent(origin.GetComponentType())
{
}

TileMap::~TileMap()
{
	//Tile Render
	m_p_mesh.reset();
	m_p_material.reset();

	//Tile
	m_p_tile_map_buffer.reset();
	m_tile_info_vector.clear();

	//Grid
	m_p_grid_mesh.reset();
	m_p_grid_material.reset();
	m_grid_left_top_vector.clear();
}

void TileMap::Start()
{

}

void TileMap::FinalUpdate()
{
    auto render_manager = RenderManager::GetInstance();
	auto screen_offset = render_manager->GetScreenOffset();
	auto editor_camera = render_manager->GetEditorCamera();

	//편집 상태이고 
	if ((SceneManager::GetInstance()->GetEditorState() == EditorState_Stop) && MOUSE_BUTTON_DOWN(KeyCode::CLICK_LEFT))
	{
		auto mouse_world_position = editor_camera->Picking();
	
		CalcCurrentPickRect(Vector2(mouse_world_position.x, mouse_world_position.y));
	}

	if (!m_tile_info_vector.empty())
		m_p_tile_map_buffer->SetStructuredBufferData(m_tile_info_vector.data(), sizeof(TileInfo) * m_tile_count);
}

void TileMap::Render()
{
	if (m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

	CalcGridCoord();
	BindPipeline();
	m_p_mesh->Render();
	DrawGrid();
}

void TileMap::CalcGridCoord()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();
	if (m_tile_size != Vector2::Zero && m_tile_count != 0)
	{
		transform->SetMeshScale(static_cast<UINT>(m_tile_count_x * m_tile_size.x), static_cast<UINT>(m_tile_count_y * m_tile_size.y));

		auto world_matrix = transform->GetWorldMatrix();

		for (UINT i = 0; i < m_tile_count; ++i)
		{
			Vector3 left_top = Vector3(m_grid_left_top_vector[i].x, m_grid_left_top_vector[i].y, 0.0f);

			left_top = left_top * world_matrix;

			m_tile_info_vector[i].left_top.x = left_top.x;
			m_tile_info_vector[i].left_top.y = left_top.y;

			Vector3 tile_size = Vector3(m_tile_size.x, m_tile_size.y, 0.0f);

			tile_size = tile_size * transform->GetLocalScale();

			m_tile_info_vector[i].right_bottom = Vector2
			(
				m_tile_info_vector[i].left_top.x + tile_size.x,
				m_tile_info_vector[i].left_top.y - tile_size.y
			);
		}
	}
	transform->UpdateConstantBuffer();
}

void TileMap::BindPipeline()
{
	//Set Tile Count X
	m_p_material->SetConstantBufferData(Material_Parameter::INT_0, &m_tile_count_x);

	//Set Tile Count Y
	m_p_material->SetConstantBufferData(Material_Parameter::INT_1, &m_tile_count_y);

	m_p_material->BindPipeline();

	if (!m_tile_info_vector.empty())
	{
		//Set Tile Map Buffer
		m_p_tile_map_buffer->SetBufferBindStage(PipelineStage::PS);
		m_p_tile_map_buffer->SetBufferBindSlot(15);
		m_p_tile_map_buffer->BindPipeline();
	}
}

void TileMap::DrawGrid()
{
	if (m_is_draw_grid && (SceneManager::GetInstance()->GetEditorState() == EditorState_Stop))
	{
		m_p_grid_material->BindPipeline();
		m_p_grid_mesh->Render();
	}
}

const std::shared_ptr<Texture>& TileMap::GetTileAtlasTexture(const UINT& tile_atlas_texture_index)
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

void TileMap::SetTileAtlasTexture(const std::shared_ptr<Texture>& p_tile_atlas_texture)
{
	if (m_p_material && m_tile_atlas_texture_count != 8)
	{
		//현재 타일맵에 사용될 텍스처 저장
		m_p_current_tile_atlas_texture = p_tile_atlas_texture;

		//이미 등록된 텍스처인지 확인

		auto texture_vector = m_p_material->GetTextureVector();

		for (UINT i = 0; i < static_cast<UINT>(texture_vector.size()); ++i)
		{
			if (texture_vector[i] != nullptr && texture_vector[i]->GetResourceName() == p_tile_atlas_texture->GetResourceName())
			{
				m_current_tile_atlas_texture_index = static_cast<int>(i);
				return;
			}
		}

		//Texture
		UINT texture_offset = static_cast<UINT>(Material_Parameter::TEX_0) + m_tile_atlas_texture_count;

		m_p_material->SetConstantBufferData(static_cast<Material_Parameter>(texture_offset), nullptr, p_tile_atlas_texture);

		//Texture Info
		//x: Texture Size X, y: Texture Size Y (Texture Size)
		//z: Texture_U , w: Texture_V (Per Tile UV Size In Texture)
		Vector4 texture_info = Vector4::Zero;

		//Set Atlas Texture Resolution
		texture_info.x = static_cast<float>(p_tile_atlas_texture->GetWidth());
		texture_info.y = static_cast<float>(p_tile_atlas_texture->GetHeight());

		//Set Atlas Textrue UV
		texture_info.z = m_tile_size.x / texture_info.x;
		texture_info.w = m_tile_size.y / texture_info.y;

		UINT texture_info_offset = static_cast<UINT>(Material_Parameter::VEC4_0) + m_tile_atlas_texture_count;

		m_p_material->SetConstantBufferData(static_cast<Material_Parameter>(texture_info_offset), &texture_info);

		m_current_tile_atlas_texture_index = static_cast<int>(m_tile_atlas_texture_count);

		++m_tile_atlas_texture_count;
	}
}

void TileMap::SetTileCount(const UINT& tile_count_x, const UINT& tile_count_y)
{
	m_tile_count_x = tile_count_x;
	m_tile_count_y = tile_count_y;

	if (!m_grid_left_top_vector.empty())
	{
		m_grid_left_top_vector.clear();
	}

	//각 타일의 정보가 이미 저장되어 있다면
	if (!m_tile_info_vector.empty())
		m_tile_info_vector.clear(); //클리어

	m_tile_count = m_tile_count_x * m_tile_count_y;
	m_tile_info_vector.resize(m_tile_count);

	m_p_grid_mesh->Create(MeshType::Grid, m_tile_count_x, m_tile_count_y);

	m_grid_left_top_vector = m_p_grid_mesh->GetGridLeftTopVector();

	for (UINT i = 0; i < m_tile_count; ++i)
	{
		m_tile_info_vector[i].tile_atlas_index = -1;
		m_tile_info_vector[i].tile_index = -1;
		m_tile_info_vector[i].left_top = m_grid_left_top_vector[i];
	}

	m_p_tile_map_buffer->Create(sizeof(TileInfo), m_tile_count, SBufferType::Read_Only, true, m_tile_info_vector.data());

	EDITOR_LOG_INFO_F("Tile Size: %dx%d", tile_count_x, tile_count_y);
}

void TileMap::CalcCurrentPickRect(const Vector2& current_screen_pos)
{
	for (int i = 0; i < static_cast<int>(m_tile_info_vector.size()); ++i)
	{
		if (RenderManager::GetInstance()->CheckMouseWorldPositionInRect(
			current_screen_pos,
			m_tile_info_vector[i].left_top,
			m_tile_info_vector[i].right_bottom))
		{
			m_tile_info_vector[i].tile_atlas_index = m_current_tile_atlas_texture_index;
			m_tile_info_vector[i].tile_index = m_current_tile_index;
		}
	}
}

void TileMap::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	auto resource_manager = ResourceManager::GetInstance();

	//Tile
	fprintf(p_file, "[Tile]\n");
	fprintf(p_file, "[Count]\n");
	fprintf(p_file, "%d\n", m_tile_count);
	fprintf(p_file, "[Column]\n");
	fprintf(p_file, "%d\n", m_tile_count_x);
	fprintf(p_file, "[Row]\n");
	fprintf(p_file, "%d\n", m_tile_count_y);
	fprintf(p_file, "[Size]\n");
	FileManager::FPrintf_Vector2(m_tile_size, p_file);

	fprintf(p_file, "[Material]\n");
	resource_manager->SaveResource<Material>(m_p_material, p_file);
	fprintf(p_file, "[Mesh]\n");
	resource_manager->SaveResource<Mesh>(m_p_mesh, p_file);

	//Grid
	fprintf(p_file, "[Grid]\n");
	fprintf(p_file, "[Draw]\n");
	fprintf(p_file, "%d\n", m_is_draw_grid);
	fprintf(p_file, "[Material]\n");
	resource_manager->SaveResource<Material>(m_p_grid_material, p_file);
	fprintf(p_file, "[Mesh]\n");
	resource_manager->SaveResource<Mesh>(m_p_grid_mesh, p_file);
	fprintf(p_file, "[Vector]\n");
	//TODO
}

void TileMap::LoadFromScene(FILE* p_file)
{
	auto resource_manager = ResourceManager::GetInstance();

	char char_buffer[256] = { 0 };

	//Tile
	FileManager::FScanf(char_buffer, p_file); //[Tile]
	FileManager::FScanf(char_buffer, p_file); //[Count]
	fscanf_s(p_file, "%d\n", &m_tile_count);
	FileManager::FScanf(char_buffer, p_file); //[Column]
	fscanf_s(p_file, "%d\n", &m_tile_count_x);
	FileManager::FScanf(char_buffer, p_file); //[Row]
	fscanf_s(p_file, "%d\n", &m_tile_count_y);
	FileManager::FScanf(char_buffer, p_file); //[Size]
	FileManager::FScanf_Vector2(m_tile_size, p_file);

	FileManager::FScanf(char_buffer, p_file); //[Material]
	resource_manager->LoadResource<Material>(m_p_material, p_file);
	FileManager::FScanf(char_buffer, p_file); //[Mesh]
	resource_manager->LoadResource<Mesh>(m_p_mesh, p_file);

	//Grid
	FileManager::FScanf(char_buffer, p_file); //[Grid]
	FileManager::FScanf(char_buffer, p_file); //[Draw]
	fscanf_s(p_file, "%d\n", &m_is_draw_grid);
	FileManager::FScanf(char_buffer, p_file); //[Material]
	resource_manager->LoadResource<Material>(m_p_grid_material, p_file);
	FileManager::FScanf(char_buffer, p_file); //[Mesh]
	resource_manager->LoadResource<Mesh>(m_p_grid_mesh, p_file);
	FileManager::FScanf(char_buffer, p_file); //[Vector]
	//TODO

	SetTileCount(m_tile_count_x, m_tile_count_y);
}
