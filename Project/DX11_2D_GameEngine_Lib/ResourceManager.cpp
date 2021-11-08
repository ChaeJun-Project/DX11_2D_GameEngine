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
	CreateShader();
	CreateMaterial();
	//Collider2D ���� Mesh ����
	CreateMesh(MeshType::Rectangle, 100, 100);
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

	auto shader_iter = m_p_shader_map.insert(std::make_pair(ShaderResourceType::Standard, shader));
	auto result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Collider2D Shader
	shader = std::make_shared<Shader>("Collider2D_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/DrawLineShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/DrawLineShader.fx", "PS", "ps_5_0");

	shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	shader->SetDepthStencilType(DepthStencilType::No_Test_No_Write); //���� �񱳸� �����ʰ� ��ϵ� ���� ����
	shader->SetBlendType(BlendType::Default);
	shader->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP); //������ �׸���

	shader_iter = m_p_shader_map.insert(std::make_pair(ShaderResourceType::Collider2D, shader));
	result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	//Create Light2D Shader
	shader = std::make_shared<Shader>("Light2D_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/LightTextureShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/LightTextureShader.fx", "PS", "ps_5_0");

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
	shader->AddAndCreateShader<VertexShader>("Shader/ParticleShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<GeometryShader>("Shader/ParticleShader.fx", "GS", "gs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/ParticleShader.fx", "PS", "ps_5_0");
	shader->AddAndCreateShader<ComputeShader>("Shader/ParticleUpdate.fx", "CS", "cs_5_0");

	shader->SetRasterizerType(RasterizerType::Cull_None_Solid);
	shader->SetDepthStencilType(DepthStencilType::Less_Equal);
	shader->SetBlendType(BlendType::Alpha_Blend);

	shader_iter = m_p_shader_map.insert(std::make_pair(ShaderResourceType::Particle, shader));
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

void ResourceManager::CreateMaterial()
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

//Texture/objectname/~~/textureName.Ȯ����
const std::shared_ptr<Texture>& ResourceManager::LoadTexture(const std::string& texture_path)
{
	//�ؽ�ó�� ���� �̸�(Ȯ���ڸ� ��)�� ����
	auto file_name = FileManager::GetIntactFileNameFromPath(texture_path);

	auto texture = GetTexture(file_name);

	//�ش� �̸��� texture�� �����ϴ� ���
	if (texture != nullptr)
		return texture;

	auto new_texture = std::make_shared<Texture>(file_name);
	new_texture->LoadFromFile(texture_path);

	//�ش� �̸��� texture�� ���� ��� => ���� ����
	//Ȯ���ڰ� ���Ե� ���� �̸����� Ȯ���� �κ��� �� string�� �ش� �ؽ�ó�� ���ҽ� �̸����� ����
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

	//�ش� �̸��� texture�� �����ϴ� ���
	if (texture != nullptr)
		return texture;

	auto new_texture = std::make_shared<Texture>(texture_name);
	new_texture->Create(width, height, texture_format, bind_flage);

	//�ش� �̸��� texture�� ���� ��� => ���� ����
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

	//�ش� �̸��� texture�� �����ϴ� ���
	if (texture != nullptr)
		return texture;

	auto new_texture = std::make_shared<Texture>(texture_name);
	new_texture->Create(texture2D);

	//�ش� �̸��� texture�� ���� ��� => ���� ����
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

	//�ش� �ؽ�ó�� ã�� ������ ���
	if (texture_iter == m_p_textrue_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter->second);
}

const std::shared_ptr<Mesh>& ResourceManager::CreateMesh(const MeshType& mesh_type, const UINT& width, const UINT& height)
{
	auto mesh = GetMesh(width, height);

	//�ش� �������� mesh�� �����ϴ� ���
	if (mesh != nullptr)
		return mesh;

	//�ش� �������� mesh�� ���� ��� => ���� ����
	//Create Standard Shader
	//�Ž��� ũ�Ⱚ���� �����ϰ� ���ҽ� �̸����� ����
	auto new_mesh = std::make_shared<Mesh>(std::to_string(width) + "x" + std::to_string(height) + "_Mesh");
	new_mesh->Create(mesh_type, width, height);

	auto mesh_iter = m_p_mesh_map.insert(std::make_pair(std::make_pair(width, height), new_mesh));
	auto result = mesh_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Mesh>(mesh_iter.first->second);
}

const std::shared_ptr<Mesh>& ResourceManager::GetMesh(const UINT& width, const UINT& height)
{
	auto mesh_iter = m_p_mesh_map.find(std::pair(width, height));

	//�ش� �������� �޽��� ã�� ������ ���
	if (mesh_iter == m_p_mesh_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Mesh>(mesh_iter->second);
}

void ResourceManager::AddPrefab(const std::string& prefab_object_name, GameObject* p_game_object)
{
	if (p_game_object == nullptr)
		return;

	//������ ������Ʈ ����(��������� ȣ��)
	Prefab* p_prefab = new Prefab(p_game_object);
	auto prefab_iter = m_p_prefab_map.insert(std::make_pair(prefab_object_name, p_prefab));
	auto result = prefab_iter.second;
	assert(result);
}

const std::shared_ptr<Prefab>& ResourceManager::GetPrefab(const std::string& game_object_name)
{
	auto prefab_iter = m_p_prefab_map.find(game_object_name);

	//�ش� ������ ������Ʈ�� ã�� ������ ���
	if (prefab_iter == m_p_prefab_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Prefab>(prefab_iter->second);
}
