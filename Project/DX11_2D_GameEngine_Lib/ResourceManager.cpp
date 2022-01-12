#include "stdafx.h"
#include "ResourceManager.h"

//Mesh
#include "Mesh.h"
//Material
#include "Material.h"
//Shader
#include "Shader.h"
#include "ComputeShader.h"
//Texture
#include "Texture.h"
//AudioClip
#include "AudioClip.h"
//Prefab
#include "Prefab.h"

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

const ResourceMap& ResourceManager::GetResourceMap(const ResourceType& resource_type)
{
	auto resource_map_iter = m_resources_map.find(resource_type);
	if (resource_map_iter == m_resources_map.end())
		return ResourceMap();

	return resource_map_iter->second;
}

void ResourceManager::Initialize()
{
	CreateResourceMap();

	CreateDefaultShader();
	CreateDefaultMaterial();
	CreateDefaultMesh();
	CreateDefaultTexture();
}

void ResourceManager::CreateResourceMap()
{
	for (UINT i = static_cast<UINT>(ResourceType::Mesh); i <= static_cast<UINT>(ResourceType::Prefab); ++i)
	{
		m_resources_map.insert(std::make_pair(static_cast<ResourceType>(i), ResourceMap()));
	}
}

void ResourceManager::CreateDefaultShader()
{
    auto& shader_map = m_resources_map[ResourceType::Shader];

	//Create Default Shader
	auto p_shader = std::make_shared<Shader>("Default");
	p_shader->AddShader<VertexShader>("Shader/TextureShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>("Shader/TextureShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::Less_Equal);
	p_shader->SetBlendType(BlendType::Alpha_Blend);

	auto shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	auto result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Line Shader
	p_shader = std::make_shared<Shader>("Line");
	p_shader->AddShader<VertexShader>("Shader/DrawLineShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>("Shader/DrawLineShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write); //깊이 비교를 하지않고 기록도 하지 않음
	p_shader->SetBlendType(BlendType::Default);
	p_shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); //선으로 그리기

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Grid Shader
	p_shader = std::make_shared<Shader>("Grid");
	p_shader->AddShader<VertexShader>("Shader/DrawLineShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>("Shader/DrawLineShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write); //깊이 비교를 하지않고 기록도 하지 않음
	p_shader->SetBlendType(BlendType::Default);
	p_shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); //모든 정점을 잇는 선으로 그리기

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Light2D Shader
	p_shader = std::make_shared<Shader>("Light2D");
	p_shader->AddShader<VertexShader>("Shader/LightTextureShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>("Shader/LightTextureShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::Less_Equal);
	p_shader->SetBlendType(BlendType::Alpha_Blend);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Particle Shader
	p_shader = std::make_shared<Shader>("Rain");
	p_shader->AddShader<VertexShader>("Shader/RainShader.fx", "VS_Rain", "vs_5_0");
	p_shader->AddShader<GeometryShader>("Shader/RainShader.fx", "GS_Rain", "gs_5_0");
	p_shader->AddShader<PixelShader>("Shader/RainShader.fx", "PS_Rain", "ps_5_0");
	p_shader->AddShader<ComputeShader>("Shader/RainUpdate.fx", "CS_Rain", "cs_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::GS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Particle);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	p_shader->SetBlendType(BlendType::Alpha_Blend);
	p_shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Post Effect Shader
	p_shader = std::make_shared<Shader>("Post_Effect");
	p_shader->AddShader<VertexShader>("Shader/PostEffectShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>("Shader/PostEffectShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::PostEffect);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	p_shader->SetBlendType(BlendType::Default);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Water Shader
	p_shader = std::make_shared<Shader>("Water");
	p_shader->AddShader<VertexShader>("Shader/WaterShader.fx", "VS_Water", "vs_5_0");
	p_shader->AddShader<PixelShader>("Shader/WaterShader.fx", "PS_Water", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::PostEffect);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	p_shader->SetBlendType(BlendType::Default);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Tile Shader
	p_shader = std::make_shared<Shader>("TileMap");
	p_shader->AddShader<VertexShader>("Shader/TileMapShader.fx", "VS", "vs_5_0");
	p_shader->AddShader<PixelShader>("Shader/TileMapShader.fx", "PS", "ps_5_0");
	p_shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	p_shader->SetRenderTimePointType(RenderTimePointType::Forward);
	p_shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	p_shader->SetDepthStencilType(DepthStencilType::Less_Equal);
	p_shader->SetBlendType(BlendType::Alpha_Blend);

	shader_iter = shader_map.insert(std::make_pair(p_shader->GetResourceName(), p_shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;
}

void ResourceManager::AddComputeShader(const std::string& compute_shader_name, const std::shared_ptr<ComputeShader>& p_compute_shader)
{
	auto compute_shader_iter = m_p_compute_shader_map.insert(std::make_pair(compute_shader_name, p_compute_shader));
	auto result = compute_shader_iter.second;
	assert(result);
	if (!result)
		return;
}

const std::shared_ptr<ComputeShader>& ResourceManager::GetComputeShader(const std::string& compute_shader_name)
{
	auto compute_shader_iter = m_p_compute_shader_map.find(compute_shader_name);

#ifdef _DEBUG
	if (compute_shader_iter == m_p_compute_shader_map.end())
		return nullptr;
#endif 
	
	return compute_shader_iter->second;
}

void ResourceManager::CreateDefaultMaterial()
{
	//=============================================
	//Default
	//=============================================
	CreateMaterial("Default_Material", "Default");

	//=============================================
	//Collider2D
	//=============================================
	CreateMaterial("Collider2D_Material", "Line");

	//=============================================
	//Grid
	//=============================================
	CreateMaterial("Grid_Material", "Grid");

	//=============================================
	//Rain
	//=============================================
	CreateMaterial("Rain_Material", "Rain");

	//=============================================
	//Water
	//=============================================
	CreateMaterial("Water_Material", "Water");

	//=============================================
	//Tile
	//=============================================
	CreateMaterial("TileMap_Material", "TileMap");
}

const std::shared_ptr<Material>& ResourceManager::CreateMaterial(const std::string& material_name, const std::string& shader_name)
{
	auto& material_map = m_resources_map[ResourceType::Material];

	auto p_material = std::make_shared<Material>(material_name);
	p_material->SetShader(GetResource<Shader>(shader_name));

	auto material_iter = material_map.insert(std::make_pair(material_name, p_material));
	auto result = material_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Material>(material_iter.first->second);
}

void ResourceManager::CreateDefaultTexture()
{
	CreateTexture("Asset/Texture/Noise/noise_01.png");

	//Noise Texture 2
	CreateTexture("Asset/Texture/Noise/noise_02.png");

	//Noise Texture 3
	CreateTexture("Asset/Texture/Noise/noise_03.jpg");

	//Noise Texture 1 사용
	auto noise_texture = GetResource<Texture>("noise_01");

	//Texture Bind Pipeline
	noise_texture->SetPipelineStage(PipelineStage::Graphics_ALL | PipelineStage::CS);
	noise_texture->SetBindSlot(13);
	noise_texture->BindPipeline();

	g_cbuffer_program.noise_resolution = Vector2
	(
		static_cast<float>(noise_texture->GetWidth()),
		static_cast<float>(noise_texture->GetHeight())
	);

	//Smoke Particle Texture
	CreateTexture("Asset/Texture/Particle/smoke_particle.png");

	//Rain Particle Texture
	CreateTexture("Asset/Texture/Particle/rain_particle.png");
}

const std::shared_ptr<Texture>& ResourceManager::CreateTexture(const std::string& texture_path)
{
	auto& texture_map = m_resources_map[ResourceType::Texture];

	std::string texture_name = FileManager::GetOriginFileNameFromPath(texture_path);

	auto p_texture = std::make_shared<Texture>(texture_name);
	p_texture->LoadFromFile(texture_path);

	auto texture_iter = texture_map.insert(std::make_pair(texture_name, p_texture));
	auto result = texture_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter.first->second);
}

const std::shared_ptr<Texture>& ResourceManager::CreateTexture(const std::string& texture_name, const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage)
{
	auto& texture_map = m_resources_map[ResourceType::Texture];

	auto p_texture = std::make_shared<Texture>(texture_name);
	p_texture->Create(width, height, texture_format, bind_flage);

	//해당 이름의 texture가 없는 경우 => 새로 생성
	auto texture_iter = texture_map.insert(std::make_pair(texture_name, p_texture));
	auto result = texture_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter.first->second);
}

const std::shared_ptr<Texture>& ResourceManager::CreateTexture(const std::string& texture_name, const ComPtr<ID3D11Texture2D>& texture2D)
{
	auto& texture_map = m_resources_map[ResourceType::Texture];

	auto p_texture = std::make_shared<Texture>(texture_name);
	p_texture->Create(texture2D);

	auto texture_iter = texture_map.insert(std::make_pair(texture_name, p_texture));
	auto result = texture_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter.first->second);
}

void ResourceManager::CreateDefaultMesh()
{
	//Point
	CreateMesh("Point_Mesh", MeshType::Point);
	//Triangle
	CreateMesh("Triangle_Mesh", MeshType::Triangle);
	//Rectangle
	CreateMesh("Rectangle_Mesh", MeshType::Rectangle);
	//Circle
	CreateMesh("Circle_Mesh", MeshType::Circle);
}

const std::shared_ptr<Mesh>& ResourceManager::CreateMesh(const std::string& mesh_name, const MeshType& mesh_type)
{
	auto& mesh_map = m_resources_map[ResourceType::Mesh];

	auto p_mesh = std::make_shared<Mesh>(mesh_name);
	p_mesh->Create(mesh_type);

	auto mesh_iter = mesh_map.insert(std::make_pair(mesh_name, p_mesh));
	auto result = mesh_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Mesh>(mesh_iter.first->second);
}

const std::shared_ptr<AudioClip>& ResourceManager::CreateAudioClip(const std::string& audio_clip_path)
{
	auto& audio_clip_map = m_resources_map[ResourceType::AudioClip];

	std::string audio_clip_name = FileManager::GetOriginFileNameFromPath(audio_clip_path);

	auto p_audio_clip = std::make_shared<AudioClip>(audio_clip_name);
	p_audio_clip->LoadFromFile(audio_clip_path);
	
	auto audio_clip_iter = audio_clip_map.insert(std::make_pair(audio_clip_name, p_audio_clip));
	auto result = audio_clip_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<AudioClip>(audio_clip_iter.first->second);
}

const std::shared_ptr<Prefab>& ResourceManager::CreatePrefab(GameObject* p_game_object)
{
	if (p_game_object == nullptr)
		return nullptr;

	auto& prefab_map = m_resources_map[ResourceType::Prefab];

	auto p_prefab = std::make_shared<Prefab>(p_game_object);

	auto prefab_iter = prefab_map.insert(std::make_pair(p_game_object->GetGameObjectName(), p_prefab));
	auto result = prefab_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Prefab>(prefab_iter.first->second);
}