#include "stdafx.h"
#include "ResourceManager.h"

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

ResourceManager::ResourceManager()
{
	Initialize();
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Initialize()
{
	CreateShader();
}

void ResourceManager::CreateShader()
{
	//Create Standard Shader
	auto shader = std::make_shared<Shader>("Standard_Shader");
	shader->AddAndCreateShader<VertexShader>("Shader/TextureShader.fx", "VS", "vs_5_0");
	shader->AddAndCreateShader<PixelShader>("Shader/TextureShader.fx", "PS", "ps_5_0");

	auto shader_iter = this->m_p_shader_map.insert(std::make_pair(ShaderResourceType::Standard, shader));
	auto result = shader_iter.second;
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

	//�ش� �ؽ�ó�� ã�� ������ ���
	if(texture_iter == this->m_p_textrue_map.end())
	  return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter->second);
}

const std::shared_ptr<Mesh>& ResourceManager::CreateMesh(const Vector2& mesh_size)
{ 
    auto mesh = GetMesh(mesh_size);

	//�ش� �������� mesh�� �����ϴ� ���
	if(mesh != nullptr)
	   return mesh;

	auto width = static_cast<UINT>(mesh_size.x);
	auto height = static_cast<UINT>(mesh_size.y);
	//�ش� �������� mesh�� ���� ��� => ���� ����
	//Create Standard Shader
	//�Ž��� ũ�Ⱚ���� �����ϰ� ���ҽ� �̸����� ����
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

	//�ش� �������� �޽��� ã�� ������ ���
	if (mesh_iter == this->m_p_mesh_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<Mesh>(mesh_iter->second);
}
