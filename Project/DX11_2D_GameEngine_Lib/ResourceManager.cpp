#include "stdafx.h"
#include "ResourceManager.h"

//Mesh
#include "Mesh.h"
//Material
#include "Material.h"
//Shader
#include "Shader.h"
//Texture
#include "Texture.h"
//Animation
#include "Animation.h"
//Prefab
#include "Prefab.h"

ResourceManager::~ResourceManager()
{
	//Shader
	for (auto& shader : m_p_shader_map)
		shader.second.reset();
	
	m_p_shader_map.clear();

	//Material
	for (auto& material : m_p_material_map)
		material.second.reset();

	m_p_material_map.clear();

	//Texture
	for (auto& texture : m_p_textrue_map)
		texture.second.reset();

	m_p_textrue_map.clear();

	//Mesh
	for (auto& mesh : m_p_mesh_map)
		mesh.second.reset();

	m_p_mesh_map.clear();

	//Prefab
	for (auto& prefab : m_p_prefab_map)
		prefab.second.reset();

	m_p_prefab_map.clear();
}

void ResourceManager::Initialize()
{
	CreateDefaultShader();
	CreateDefaultMaterial();
	CreateDefaultMesh();
	CreateDefaultTexture();
}

void ResourceManager::CreateDefaultShader()
{
	//Create Standard Shader
	auto shader = std::make_shared<Shader>("Standard_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/TextureShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/TextureShader.fx", "PS", "ps_5_0");
	shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	shader->SetRenderTimePointType(RenderTimePointType::Forward);
	shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	shader->SetDepthStencilType(DepthStencilType::Less_Equal);
	shader->SetBlendType(BlendType::Alpha_Blend);

	auto shader_iter = m_p_shader_map.insert(std::make_pair(ShaderResourceType::Standard, shader));
	auto result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Collider2D Shader
	shader = std::make_shared<Shader>("Collider2D_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/DrawLineShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/DrawLineShader.fx", "PS", "ps_5_0");
	shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	shader->SetRenderTimePointType(RenderTimePointType::Forward);
	shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write); //깊이 비교를 하지않고 기록도 하지 않음
	shader->SetBlendType(BlendType::Default);
	shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); //선으로 그리기

	shader_iter = m_p_shader_map.insert(std::make_pair(ShaderResourceType::Collider2D, shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Light2D Shader
	shader = std::make_shared<Shader>("Light2D_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/LightTextureShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/LightTextureShader.fx", "PS", "ps_5_0");
	shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	shader->SetRenderTimePointType(RenderTimePointType::Forward);
	shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	shader->SetDepthStencilType(DepthStencilType::Less_Equal);
	shader->SetBlendType(BlendType::Alpha_Blend);

	shader_iter = m_p_shader_map.insert(std::make_pair(ShaderResourceType::Light2D, shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Particle Shader
	shader = std::make_shared<Shader>("Particle_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/RainShader.fx", "VS_Rain", "vs_5_0");
	shader->AddAndCreateShader<GeometryShader>("Shader/RainShader.fx", "GS_Rain", "gs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/RainShader.fx", "PS_Rain", "ps_5_0");
	shader->AddAndCreateShader<ComputeShader>("Shader/RainUpdate.fx", "CS_Rain", "cs_5_0");
	shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::GS | PipelineStage::PS | PipelineStage::CS);

	shader->SetRenderTimePointType(RenderTimePointType::Particle);
	shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	shader->SetBlendType(BlendType::Alpha_Blend);
	shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	shader_iter = m_p_shader_map.insert(std::make_pair(ShaderResourceType::Particle, shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Post Effect Shader
	shader = std::make_shared<Shader>("Post_Effect_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/PostEffectShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/PostEffectShader.fx", "PS", "ps_5_0");
	shader->SetShaderBindStage(PipelineStage::VS | PipelineStage::PS);

	shader->SetRenderTimePointType(RenderTimePointType::Post_Effect);
	shader->SetRasterizerType(RasterizerType::Cull_Back_Solid);
	shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write);
	shader->SetBlendType(BlendType::Default);
	shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	shader_iter = m_p_shader_map.insert(std::make_pair(ShaderResourceType::Post_Effect, shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;
}

const std::shared_ptr<Shader>& ResourceManager::GetShaderResource(const ShaderResourceType& shader_type)
{
	auto shader_iter = m_p_shader_map.find(shader_type);

	if (shader_iter == m_p_shader_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Shader>(shader_iter->second);
}

void ResourceManager::CreateDefaultMaterial()
{
    //=============================================
    //Collider2D
	//=============================================
	//Collider2D Material White
	std::string material_name = "Collider2D_White";
	auto material = std::make_shared<Material>(material_name);
	material->SetShader(GetShaderResource(ShaderResourceType::Collider2D));

	auto material_iter = m_p_material_map.insert(std::make_pair(material_name, material));
	auto result = material_iter.second;
	assert(result);
	if (!result)
		return;

	//Collider2D Material Green
	material_name = "Collider2D_Green";
	material = std::make_shared<Material>(material_name);
	material->SetShader(GetShaderResource(ShaderResourceType::Collider2D));

	int a = 1;
	material->SetConstantBufferData(Material_Parameter::INT_0, &a, nullptr);

	material_iter = m_p_material_map.insert(std::make_pair(material_name, material));
	result = material_iter.second;
	assert(result);
	if (!result)
		return;

	//Collider2D Material Red
	material_name = "Collider2D_Red";
	material = std::make_shared<Material>(material_name);
	material->SetShader(GetShaderResource(ShaderResourceType::Collider2D));

	a = 1;
	material->SetConstantBufferData(Material_Parameter::INT_1, &a, nullptr);

	material_iter = m_p_material_map.insert(std::make_pair(material_name, material));
	result = material_iter.second;
	assert(result);
	if (!result)
		return;

	//=============================================
	//Particle
	//=============================================
	material_name = "Particle";
	material = std::make_shared<Material>(material_name);
	material->SetShader(GetShaderResource(ShaderResourceType::Particle));

	material_iter = m_p_material_map.insert(std::make_pair(material_name, material));
	result = material_iter.second;
	assert(result);
	if (!result)
		return;
}

const std::shared_ptr<Material>& ResourceManager::GetMaterial(const std::string& material_name)
{
	auto material_iter = m_p_material_map.find(material_name);

	if (material_iter == m_p_material_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Material>(material_iter->second);
}

void ResourceManager::CreateDefaultTexture()
{
    //Noise Texture 1
	LoadTexture("Texture/Noise/noise_01.png");

	//Noise Texture 2
	LoadTexture("Texture/Noise/noise_02.png");

	//Noise Texture 1 사용
	auto noise_texture = GetTexture("noise_01");

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
	LoadTexture("Texture/Particle/smoke_particle.png");

	//Rain Particle Texture
	LoadTexture("Texture/Particle/rain_particle.png");
}

//Texture/objectname/~~/textureName.확장자
const std::shared_ptr<Texture>& ResourceManager::LoadTexture(const std::string& texture_path)
{
	//텍스처의 순수 이름(확장자를 뺀)을 구함
	auto file_name = FileManager::GetIntactFileNameFromPath(texture_path);

	auto texture = GetTexture(file_name);

	//해당 이름의 texture가 존재하는 경우
	if (texture != nullptr)
		return texture;

	auto new_texture = std::make_shared<Texture>(file_name);
	new_texture->LoadFromFile(texture_path);

	//해당 이름의 texture가 없는 경우 => 새로 생성
	//확장자가 포함된 파일 이름에서 확장자 부분을 뺀 string을 해당 텍스처의 리소스 이름으로 설정
	auto texture_iter = m_p_textrue_map.insert(std::make_pair(file_name, new_texture));
	auto result = texture_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter.first->second);
}

const std::shared_ptr<Texture>& ResourceManager::CreateTexture(const std::string& texture_name, const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage)
{
	auto texture = GetTexture(texture_name);

	//해당 이름의 texture가 존재하는 경우
	if (texture != nullptr)
		return texture;

	auto new_texture = std::make_shared<Texture>(texture_name);
	new_texture->Create(width, height, texture_format, bind_flage);

	//해당 이름의 texture가 없는 경우 => 새로 생성
	auto texture_iter = m_p_textrue_map.insert(std::make_pair(texture_name, new_texture));
	auto result = texture_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter.first->second);
}

const std::shared_ptr<Texture>& ResourceManager::CreateTexture(const std::string& texture_name, const ComPtr<ID3D11Texture2D>& texture2D)
{
	auto texture = GetTexture(texture_name);

	//해당 이름의 texture가 존재하는 경우
	if (texture != nullptr)
		return texture;

	auto new_texture = std::make_shared<Texture>(texture_name);
	new_texture->Create(texture2D);

	//해당 이름의 texture가 없는 경우 => 새로 생성
	auto texture_iter = m_p_textrue_map.insert(std::make_pair(texture_name, new_texture));
	auto result = texture_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter.first->second);
}

const std::shared_ptr<Texture>& ResourceManager::GetTexture(const std::string& texture_name)
{
	auto texture_iter = m_p_textrue_map.find(texture_name);

	//해당 텍스처를 찾지 못했을 경우
	if (texture_iter == m_p_textrue_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter->second);
}


void ResourceManager::CreateDefaultMesh()
{
    //Point
	CreateMesh(MeshType::Point);
	//Triangle
	CreateMesh(MeshType::Triangle);
	//Rectangle
	CreateMesh(MeshType::Rectangle);
	//Circle
	CreateMesh(MeshType::Circle);
}

const std::shared_ptr<Mesh>& ResourceManager::CreateMesh(const MeshType& mesh_type)
{
	auto mesh = GetMesh(mesh_type);

	//해당 종류의 mesh가 존재하는 경우
	if (mesh != nullptr)
		return mesh;

	std::string mesh_type_str;

	switch (mesh_type)
	{
	case MeshType::Point:
		mesh_type_str = "Point";
		break;
	case MeshType::Triangle:
		mesh_type_str = "Triangle";
		break;
	case MeshType::Rectangle:
		mesh_type_str = "Rectangle";
		break;
	case MeshType::Circle:
		mesh_type_str = "Circle";
		break;
	}

	//해당 종류의 mesh가 없는 경우 => 새로 생성
	//Create Standard Shader
	auto new_mesh = std::make_shared<Mesh>( mesh_type_str + "Mesh");
	new_mesh->Create(mesh_type);

	auto mesh_iter = m_p_mesh_map.insert(std::make_pair(mesh_type, new_mesh));
	auto result = mesh_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Mesh>(mesh_iter.first->second);
}

const std::shared_ptr<Mesh>& ResourceManager::GetMesh(const MeshType& mesh_type)
{
	auto mesh_iter = m_p_mesh_map.find(mesh_type);

	//해당 종류의 메쉬를 찾지 못했을 경우
	if (mesh_iter == m_p_mesh_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Mesh>(mesh_iter->second);
}

void ResourceManager::AddPrefab(const std::string& prefab_object_name, GameObject* p_game_object)
{
	if (p_game_object == nullptr)
		return;

	//프리팹 오브젝트 생성(복사생성자 호출)
	Prefab* p_prefab = new Prefab(p_game_object);
	auto prefab_iter = m_p_prefab_map.insert(std::make_pair(prefab_object_name, p_prefab));
	auto result = prefab_iter.second;
	assert(result);
}

const std::shared_ptr<Prefab>& ResourceManager::GetPrefab(const std::string& game_object_name)
{
	auto prefab_iter = m_p_prefab_map.find(game_object_name);

	//해당 프리팹 오브젝트를 찾지 못했을 경우
	if (prefab_iter == m_p_prefab_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Prefab>(prefab_iter->second);
}
