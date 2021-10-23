#include "stdafx.h"
#include "ResourceManager.h"

#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "Prefab.h"

ResourceManager::ResourceManager()
{
	Initialize();
}

ResourceManager::~ResourceManager()
{
	//Shader
	for (auto& shader : this->m_p_shader_map)
		shader.second.reset();
	
	this->m_p_shader_map.clear();

	//Material
	for (auto& material : this->m_p_material_map)
		material.second.reset();

	this->m_p_material_map.clear();

	//Texture
	for (auto& texture : this->m_p_textrue_map)
		texture.second.reset();

	this->m_p_textrue_map.clear();

	//Mesh
	for (auto& mesh : this->m_p_mesh_map)
		mesh.second.reset();

	this->m_p_mesh_map.clear();

	//Prefab
	for (auto& prefab : this->m_p_prefab_map)
		prefab.second.reset();

	this->m_p_prefab_map.clear();
}

void ResourceManager::Initialize()
{
	CreateShader();
	CreateMaterial();
	//Collider2D 전용 Mesh 생성
	CreateMesh(Vector2(100.0f, 100.0f));
}

void ResourceManager::CreateShader()
{
	//Create Standard Shader
	auto shader = std::make_shared<Shader>("Standard_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/TextureShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/TextureShader.fx", "PS", "ps_5_0");

	shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	shader->SetDepthStencilType(DepthStencilType::Less_Equal);
	shader->SetBlendType(BlendType::Alpha_Blend);

	auto shader_iter = this->m_p_shader_map.insert(std::make_pair(ShaderResourceType::Standard, shader));
	auto result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Collider2D Shader
	shader = std::make_shared<Shader>("Collider2D_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/DrawLineShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/DrawLineShader.fx", "PS", "ps_5_0");

	shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write); //깊이 비교를 하지않고 기록도 하지 않음
	shader->SetBlendType(BlendType::Default);
	shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); //선으로 그리기

	shader_iter = this->m_p_shader_map.insert(std::make_pair(ShaderResourceType::Collider2D, shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;
}

const std::shared_ptr<Shader>& ResourceManager::GetShaderResource(const ShaderResourceType& shader_type)
{
	auto shader_iter = this->m_p_shader_map.find(shader_type);

	if (shader_iter == this->m_p_shader_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Shader>(shader_iter->second);
}

void ResourceManager::CreateMaterial()
{
	//Collider2D Material White
	std::string material_name = "Collider2D_White";
	auto material = std::make_shared<Material>(material_name);
	material->SetShader(GetShaderResource(ShaderResourceType::Collider2D));

	auto material_iter = this->m_p_material_map.insert(std::make_pair(material_name, material));
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

	material_iter = this->m_p_material_map.insert(std::make_pair(material_name, material));
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

	material_iter = this->m_p_material_map.insert(std::make_pair(material_name, material));
	result = material_iter.second;
	assert(result);
	if (!result)
		return;
}

const std::shared_ptr<Material>& ResourceManager::GetMaterialResource(const std::string& material_name)
{
	auto material_iter = this->m_p_material_map.find(material_name);

	if (material_iter == this->m_p_material_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Material>(material_iter->second);
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
	auto texture_iter = this->m_p_textrue_map.insert(std::make_pair(file_name, new_texture));
	auto result = texture_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter.first->second);
}

const std::shared_ptr<Texture>& ResourceManager::GetTexture(const std::string& texture_name)
{
	auto texture_iter = this->m_p_textrue_map.find(texture_name);

	//해당 텍스처를 찾지 못했을 경우
	if (texture_iter == this->m_p_textrue_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter->second);
}

const std::shared_ptr<Mesh>& ResourceManager::CreateMesh(const Vector2& mesh_size)
{
	auto mesh = GetMesh(mesh_size);

	//해당 사이즈의 mesh가 존재하는 경우
	if (mesh != nullptr)
		return mesh;

	auto width = static_cast<UINT>(mesh_size.x);
	auto height = static_cast<UINT>(mesh_size.y);
	//해당 사이즈의 mesh가 없는 경우 => 새로 생성
	//Create Standard Shader
	//매쉬의 크기값으로 저장하고 리소스 이름으로 설정
	auto new_mesh = std::make_shared<Mesh>(std::to_string(width) + "x" + std::to_string(height) + "_Mesh");
	new_mesh->Create(MeshType::Rectangle, mesh_size);

	auto mesh_iter = this->m_p_mesh_map.insert(std::make_pair(std::make_pair(width, height), new_mesh));
	auto result = mesh_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Mesh>(mesh_iter.first->second);
}

const std::shared_ptr<Mesh>& ResourceManager::GetMesh(const Vector2& mesh_size)
{
	auto width = static_cast<UINT>(mesh_size.x);
	auto height = static_cast<UINT>(mesh_size.y);
	auto mesh_iter = this->m_p_mesh_map.find(std::pair(width, height));

	//해당 사이즈의 메쉬를 찾지 못했을 경우
	if (mesh_iter == this->m_p_mesh_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Mesh>(mesh_iter->second);
}

void ResourceManager::AddPrefab(const std::string& prefab_object_name, GameObject* p_game_object)
{
	if (p_game_object == nullptr)
		return;

	//프리팹 오브젝트 생성(복사생성자 호출)
	Prefab* p_prefab = new Prefab(p_game_object);
	auto prefab_iter = this->m_p_prefab_map.insert(std::make_pair(prefab_object_name, p_prefab));
	auto result = prefab_iter.second;
	assert(result);
}

const std::shared_ptr<Prefab>& ResourceManager::GetPrefab(const std::string& game_object_name)
{
	auto prefab_iter = this->m_p_prefab_map.find(game_object_name);

	//해당 프리팹 오브젝트를 찾지 못했을 경우
	if (prefab_iter == this->m_p_prefab_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Prefab>(prefab_iter->second);
}
