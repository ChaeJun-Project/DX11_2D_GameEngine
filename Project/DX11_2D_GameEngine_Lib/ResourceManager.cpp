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
	auto file_name = FileManager::GetIntactFileNameFromPath(texture_path);
	//확장자가 포함된 파일 이름에서 확장자 부분을 뺀 string을 해당 텍스처의 리소스 이름으로 설정
	auto texture_iter = this->m_p_textrue_map.insert(std::make_pair(file_name, std::make_shared<Texture>(file_name)));
	auto result = texture_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	auto texture = texture_iter.first->second;
	texture->LoadFromFile(texture_path);

	return std::dynamic_pointer_cast<Texture>(texture);
}

const std::shared_ptr<Mesh>& ResourceManager::CreateMesh(const Vector2& mesh_size)
{
	auto width = static_cast<UINT>(mesh_size.x);
	auto height = static_cast<UINT>(mesh_size.y);

	//Create Standard Shader
	//매쉬의 크기값으로 저장하고 리소스 이름으로 설정
	auto mesh = std::make_shared<Mesh>(std::to_string(width) + "x" + std::to_string(height) + "_Mesh");
	mesh->Create(MeshType::Rectangle, mesh_size);

	auto mesh_iter = this->m_p_mesh_map.insert(std::make_pair(std::make_pair(width, height), mesh));
	auto result = mesh_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	return std::dynamic_pointer_cast<Mesh>(mesh);
}
