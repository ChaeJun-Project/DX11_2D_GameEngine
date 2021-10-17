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
	if(texture_iter == this->m_p_textrue_map.end())
	  return nullptr;

	return std::dynamic_pointer_cast<Texture>(texture_iter->second);
}

const std::shared_ptr<Mesh>& ResourceManager::CreateMesh(const Vector2& mesh_size)
{ 
    auto mesh = GetMesh(mesh_size);

	//해당 사이즈의 mesh가 존재하는 경우
	if(mesh != nullptr)
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
