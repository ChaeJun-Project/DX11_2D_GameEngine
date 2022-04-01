#include "stdafx.h"
#include "ResourceManager.h"

//Mesh
#include "Mesh.h"
//Material
#include "Material.h"
//Shader
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "Shader.h"
//Texture
#include "Texture.h"
//AudioClip
#include "AudioClip.h"
//Prefab
#include "Prefab.h"
//SpriteAnimation
#include "SpriteAnimation.h"
//Particle
#include "Particle.h"
//TileMap
#include "TileMap.h"

#include "GameObject.h"

ResourceManager::~ResourceManager()
{
	//Clear Resource Map
	for (auto& resource_map : m_resources_map)
	{
		//Clear Each Resource Map
		for (auto& resource : resource_map.second)
			resource.second.reset();

		resource_map.second.clear();
	}

	m_resources_map.clear();

	//Compute Shader
	for (auto& compute_shader : m_p_compute_shader_map)
		compute_shader.second.reset();

	m_p_compute_shader_map.clear();
}

template<typename T>
inline constexpr ResourceType ResourceManager::GetResourceType()
{
	return ResourceType::NONE;
}

#define REGISTER_RESOURCE_TYPE(T, resource_type) template<> ResourceType ResourceManager::GetResourceType<T>() { return resource_type; } 

//TODO: 추후에 다른 Resource들도 추가 예정
REGISTER_RESOURCE_TYPE(Mesh, ResourceType::Mesh);
REGISTER_RESOURCE_TYPE(Material, ResourceType::Material);
REGISTER_RESOURCE_TYPE(Shader, ResourceType::Shader);
REGISTER_RESOURCE_TYPE(Texture, ResourceType::Texture);
REGISTER_RESOURCE_TYPE(AudioClip, ResourceType::AudioClip);
REGISTER_RESOURCE_TYPE(Prefab, ResourceType::Prefab);
REGISTER_RESOURCE_TYPE(SpriteAnimation, ResourceType::SpriteAnimation);
REGISTER_RESOURCE_TYPE(Particle, ResourceType::Particle);
REGISTER_RESOURCE_TYPE(TileMap, ResourceType::TileMap);

const ResourceMap ResourceManager::GetResourceMap(const ResourceType& resource_type)
{
	auto resource_map_iter = m_resources_map.find(resource_type);
	if (resource_map_iter == m_resources_map.end())
		return ResourceMap();

	return resource_map_iter->second;
}

void ResourceManager::Initialize()
{
	CreateResourceMap();

	CreateDefaultTexture();
	CreateDefaultShader();

	CreateAddedMaterial();
	CreateAddedMesh();
	CreateAddedPrefab();
	CreateAddedParticle();
}

void ResourceManager::CreateResourceMap()
{
	for (UINT i = static_cast<UINT>(ResourceType::Mesh); i <= static_cast<UINT>(ResourceType::TileMap); ++i)
	{
		m_resources_map.insert(std::make_pair(static_cast<ResourceType>(i), ResourceMap()));
	}
}

void ResourceManager::CreateDefaultShader()
{
	auto& shader_map = m_resources_map[ResourceType::Shader];

	auto absolute_content_path = FILE_MANAGER->GetAbsoluteContentPath();

	//Create Default Shader
	auto p_shader = std::make_shared<Shader>("Default");
	p_shader->AddShader<VertexShader>(absolute_content_path + "Shader/TextureShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>(absolute_content_path + "Shader/TextureShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::Less_Equal);
	p_shader->SetBlendType(BlendType::Alpha_Blend);

	auto shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	auto result = shader_iter.second;

	if (!result)
		return;

	//Create Line Shader
	p_shader = std::make_shared<Shader>("Line");
	p_shader->AddShader<VertexShader>(absolute_content_path + "Shader/DrawLineShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>(absolute_content_path + "Shader/DrawLineShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write); //깊이 비교를 하지않고 기록도 하지 않음
	p_shader->SetBlendType(BlendType::Default);
	p_shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); //선으로 그리기

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;

	if (!result)
		return;

	//Create Grid Shader
	p_shader = std::make_shared<Shader>("Grid");
	p_shader->AddShader<VertexShader>(absolute_content_path + "Shader/DrawLineShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>(absolute_content_path + "Shader/DrawLineShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write); //깊이 비교를 하지않고 기록도 하지 않음
	p_shader->SetBlendType(BlendType::Default);
	p_shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); //모든 정점을 잇는 선으로 그리기

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;

	if (!result)
		return;

	//Create Tile Shader
	p_shader = std::make_shared<Shader>("TileMapRenderer");
	p_shader->AddShader<VertexShader>(absolute_content_path + "Shader/TileMapShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>(absolute_content_path + "Shader/TileMapShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::Less_Equal);
	p_shader->SetBlendType(BlendType::Alpha_Blend);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;

	if (!result)
		return;

	//Create Snow Particle Shader
	p_shader = std::make_shared<Shader>("Snow");
	p_shader->AddShader<VertexShader>(absolute_content_path + "Shader/SnowShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<GeometryShader>(absolute_content_path + "Shader/SnowShader.fx", "GS", "gs_5_0");
	p_shader->AddShader<PixelShader>(absolute_content_path + "Shader/SnowShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::GS | PipelineStage::PS);

	p_shader->AddShader<ComputeShader>(absolute_content_path + "Shader/SnowUpdate.fx", "CS", "cs_5_0");
	AddComputeShader("Snow", p_shader->GetShader<ComputeShader>());
	
	p_shader->SetRenderTimePointType(RenderTimePointType::Particle);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	p_shader->SetBlendType(BlendType::Alpha_Blend);
	p_shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;

	if (!result)
		return;

	//Create Light2D Shader
	p_shader = std::make_shared<Shader>("Light2D");
	p_shader->AddShader<VertexShader>(absolute_content_path + "Shader/Light2DShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>(absolute_content_path + "Shader/Light2DShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::PostEffect);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	p_shader->SetBlendType(BlendType::Alpha_Blend);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;

	if (!result)
		return;

	//Create Distortion Shader
	p_shader = std::make_shared<Shader>("Distortion");
	p_shader->AddShader<VertexShader>(absolute_content_path + "Shader/DistortionShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>(absolute_content_path + "Shader/DistortionShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::PostEffect);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	p_shader->SetBlendType(BlendType::Default);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;

	if (!result)
		return;

	//Create Water Shader
	p_shader = std::make_shared<Shader>("Water");
	p_shader->AddShader<VertexShader>(absolute_content_path + "Shader/WaterShader.fx", "VS_Water", "vs_5_0");
	p_shader->AddShader<PixelShader>(absolute_content_path + "Shader/WaterShader.fx", "PS_Water", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::PostEffect);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	p_shader->SetBlendType(BlendType::Default);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;

	if (!result)
		return;

	//Create Widget Image Shader
	p_shader = std::make_shared<Shader>("ImageRenderer");
	p_shader->AddShader<VertexShader>(absolute_content_path + "Shader/ImageShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>(absolute_content_path + "Shader/ImageShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	p_shader->SetBlendType(BlendType::Alpha_Blend);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;

	if (!result)
		return;
}

void ResourceManager::AddComputeShader(const std::string& compute_shader_name, const std::shared_ptr<ComputeShader>& p_compute_shader)
{
	m_p_compute_shader_map.insert(std::make_pair(compute_shader_name, p_compute_shader));
}

const std::shared_ptr<ComputeShader>& ResourceManager::GetComputeShader(const std::string& compute_shader_name)
{
	auto compute_shader_iter = m_p_compute_shader_map.find(compute_shader_name);

	if (compute_shader_iter == m_p_compute_shader_map.end())
		return nullptr;

	return compute_shader_iter->second;
}

void ResourceManager::CreateAddedMaterial()
{
	auto material_directory_path = FILE_MANAGER->GetAbsoluteMaterialPath();
	auto material_directory_file_path_vector = FILE_MANAGER->GetFilesInDirectory(material_directory_path);
	for (const auto& material_file_path : material_directory_file_path_vector)
		LoadFromFile<Material>(material_file_path);
}

const std::shared_ptr<Material> ResourceManager::CreateMaterial(const std::string& material_name, const std::string& shader_name)
{
	auto& material_map = m_resources_map[ResourceType::Material];

	auto p_material = std::make_shared<Material>(material_name);
	p_material->SetShader(GetResource<Shader>(shader_name));

	auto material_iter = material_map.insert(std::make_pair(material_name, p_material));
	auto result = material_iter.second;
	if (!result)
		return std::dynamic_pointer_cast<Material>(material_map.find(material_name)->second);

	return std::dynamic_pointer_cast<Material>(material_iter.first->second);
}

void ResourceManager::CreateDefaultTexture()
{
	auto absolute_texture_path = TEXTURE_PATH;

	//Noise Texture 1
	LoadFromFile<Texture>(absolute_texture_path + "Noise/noise_01.png");

	//Noise Texture 2
	LoadFromFile<Texture>(absolute_texture_path + "Noise/noise_02.png");

	//Noise Texture 3
	LoadFromFile<Texture>(absolute_texture_path + "Noise/noise_03.jpg");

	//Noise Texture 1 사용
	auto noise_texture = GetResource<Texture>("noise_01");

	if (noise_texture != nullptr)
	{
		//Texture Bind Pipeline
		noise_texture->SetPipelineStage(PipelineStage::Graphics_ALL | PipelineStage::CS);
		noise_texture->SetBindSlot(13);
		noise_texture->BindPipeline();

		g_cbuffer_program.noise_resolution = Vector2
		(
			static_cast<float>(noise_texture->GetWidth()),
			static_cast<float>(noise_texture->GetHeight())
		);
	}
}

const std::shared_ptr<Texture> ResourceManager::CreateIconTexture(const std::string& icon_texture_path)
{
	std::string icon_texture_name = FILE_MANAGER->GetOriginFileNameFromPath(icon_texture_path);
	auto file_name = FILE_MANAGER->GetFileNameFromPath(icon_texture_path);

	auto p_icon_texture = std::make_shared<Texture>(icon_texture_name);
	p_icon_texture->SetResourcePath(icon_texture_path);

	if (!p_icon_texture->LoadFromFile(icon_texture_path))
	{
		return nullptr;
	}

	return p_icon_texture;
}

const std::shared_ptr<Texture> ResourceManager::CreateFileItemThumbnailTexture(const std::string& file_path)
{
	std::string texture_name = FILE_MANAGER->GetOriginFileNameFromPath(file_path);
	auto file_name = FILE_MANAGER->GetFileNameFromPath(file_path);

	auto p_thumbnail_texture = std::make_shared<Texture>(texture_name);
	p_thumbnail_texture->SetResourcePath(file_path);

	if (!p_thumbnail_texture->LoadFromFile(file_path))
	{
		return nullptr;
	}

	return p_thumbnail_texture;
}

const std::shared_ptr<Texture> ResourceManager::CreateTexture(const std::string& texture_name, const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage)
{
	auto& texture_map = m_resources_map[ResourceType::Texture];

	auto p_texture = std::make_shared<Texture>(texture_name);
	p_texture->Create(width, height, texture_format, bind_flage);

	//해당 이름의 texture가 없는 경우 => 새로 생성
	auto texture_iter = texture_map.insert(std::make_pair(texture_name, p_texture));
	auto result = texture_iter.second;
	if (!result)
		return std::dynamic_pointer_cast<Texture>(texture_map.find(texture_name)->second);

	return std::dynamic_pointer_cast<Texture>(texture_iter.first->second);
}

const std::shared_ptr<Texture> ResourceManager::CreateTexture(const std::string& texture_name, const ComPtr<ID3D11Texture2D>& texture2D)
{
	auto& texture_map = m_resources_map[ResourceType::Texture];

	auto p_texture = std::make_shared<Texture>(texture_name);
	p_texture->Create(texture2D);

	auto texture_iter = texture_map.insert(std::make_pair(texture_name, p_texture));
	auto result = texture_iter.second;
	if (!result)
		return std::dynamic_pointer_cast<Texture>(texture_map.find(texture_name)->second);

	return std::dynamic_pointer_cast<Texture>(texture_iter.first->second);
}

void ResourceManager::CreateAddedMesh()
{
	auto mesh_directory_path = FILE_MANAGER->GetAbsoluteMeshPath();
	auto mesh_directory_file_path_vector = FILE_MANAGER->GetFilesInDirectory(mesh_directory_path);
	for (const auto& mesh_file_path : mesh_directory_file_path_vector)
		LoadFromFile<Mesh>(mesh_file_path);
}

const std::shared_ptr<Mesh> ResourceManager::CreateMesh(const std::string& mesh_name, const MeshType& mesh_type)
{
	auto& mesh_map = m_resources_map[ResourceType::Mesh];

	auto p_mesh = std::make_shared<Mesh>(mesh_name);
	p_mesh->Create(mesh_type);

	auto mesh_iter = mesh_map.insert(std::make_pair(mesh_name, p_mesh));
	auto result = mesh_iter.second;
	if (!result)
		return std::dynamic_pointer_cast<Mesh>(mesh_map.find(mesh_name)->second);

	return std::dynamic_pointer_cast<Mesh>(mesh_iter.first->second);
}

void ResourceManager::CreateAddedPrefab()
{
	auto prefab_path = FILE_MANAGER->GetAbsolutePrefabPath();
	auto directory_path_vector = FILE_MANAGER->GetFilesInDirectory(prefab_path);
	for (const auto& directory_path : directory_path_vector)
		LoadFromFile<Prefab>(directory_path);
}

const std::shared_ptr<Prefab> ResourceManager::CreatePrefab(GameObject* p_game_object)
{
	if (p_game_object == nullptr)
		return nullptr;

	auto& prefab_map = m_resources_map[ResourceType::Prefab];

	auto p_prefab = std::make_shared<Prefab>(p_game_object);
	std::string resource_path = FILE_MANAGER->GetAbsolutePrefabPath();
	resource_path += (p_game_object->GetGameObjectName() + ".prefab");
	resource_path = FILE_MANAGER->GetRelativeResourcePathFromAbsolutePath(resource_path);
	p_prefab->SetResourcePath(resource_path);

	auto prefab_iter = prefab_map.insert(std::make_pair(p_game_object->GetGameObjectName(), p_prefab));
	auto result = prefab_iter.second;
	//같은 이름으로 기존 프리팹이 존재하는 경우
	//삭제 후 재생성
	if (!result) 
	{
		auto& p_update_prefab = prefab_map[p_game_object->GetGameObjectName()];
		p_update_prefab.reset();
		p_update_prefab = p_prefab;
	}

	return std::dynamic_pointer_cast<Prefab>(prefab_iter.first->second);
}

const std::shared_ptr<SpriteAnimation> ResourceManager::CreateSpriteAnimation(const std::string& animation2D_name)
{
	auto& animation2D_map = m_resources_map[ResourceType::SpriteAnimation];

	auto p_animation2D = std::make_shared<SpriteAnimation>(animation2D_name);

	auto animation2D_iter = animation2D_map.insert(std::make_pair(animation2D_name, p_animation2D));
	auto result = animation2D_iter.second;
	if (!result)
		return std::dynamic_pointer_cast<SpriteAnimation>(animation2D_map.find(animation2D_name)->second);

	return std::dynamic_pointer_cast<SpriteAnimation>(animation2D_iter.first->second);
}

void ResourceManager::CreateAddedParticle()
{
	auto particle_directory_path = FILE_MANAGER->GetAbsoluteParticlePath();
	auto particle_directory_file_path_vector = FILE_MANAGER->GetFilesInDirectory(particle_directory_path);
	for (const auto& particle_file_path : particle_directory_file_path_vector)
		LoadFromFile<Particle>(particle_file_path);
}

const std::shared_ptr<Particle> ResourceManager::CreateParticle(const std::string& particle_name)
{
	auto& particle_map = m_resources_map[ResourceType::Particle];

	auto p_particle = std::make_shared<Particle>(particle_name);

	/*auto particle_directory_path = FILE_MANAGER->GetAbsoluteParticlePath();
	particle_directory_path += particle_name;
	particle_directory_path += ".particle";
	p_particle->SetResourcePath(particle_directory_path);

	p_particle->SaveToFile(particle_directory_path);*/

	auto particle_iter = particle_map.insert(std::make_pair(particle_name, p_particle));
	auto result = particle_iter.second;
	if (!result)
		return std::dynamic_pointer_cast<Particle>(particle_map.find(particle_name)->second);

	return std::dynamic_pointer_cast<Particle>(particle_iter.first->second);
}

const std::shared_ptr<TileMap> ResourceManager::CreateTileMap(const std::string& tile_map_name)
{
	auto& tile_map = m_resources_map[ResourceType::TileMap];

	auto p_tile_map = std::make_shared<TileMap>(tile_map_name);

	auto tile_map_iter = tile_map.insert(std::make_pair(tile_map_name, p_tile_map));
	auto result = tile_map_iter.second;
	if (!result)
	{
		EDITOR_LOG_ERROR_F("The [%s] TileMap Already Exists", tile_map_name.c_str());
		return std::dynamic_pointer_cast<TileMap>(tile_map.find(tile_map_name)->second);
	}

	EDITOR_LOG_INFO_F("Succeeded in Creating TileMap: [%s]", tile_map_name.c_str());
	return std::dynamic_pointer_cast<TileMap>(tile_map_iter.first->second);
}
