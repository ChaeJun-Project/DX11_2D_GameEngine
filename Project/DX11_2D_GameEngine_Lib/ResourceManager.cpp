#include "stdafx.h"
#include "ResourceManager.h"

template<typename T>
constexpr ResourceType GetResourceType()
{
	return ResourceType::NONE;
}

#define REGISTER_RESOURCE_TYPE(T, resource_type) template<>  ResourceType ResourceManager::GetResourceType<T>() { return resource_type; }

REGISTER_RESOURCE_TYPE(Mesh, ResourceType::Mesh);
REGISTER_RESOURCE_TYPE(Material, ResourceType::Material);
REGISTER_RESOURCE_TYPE(Shader, ResourceType::Shader);
REGISTER_RESOURCE_TYPE(Texture, ResourceType::Texture);
REGISTER_RESOURCE_TYPE(Animation, ResourceType::Animation);

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
	CreateMaterial();
}

void ResourceManager::CreateShader()
{
	//Create Standard Shader
	auto shader_iter = this->m_p_shader_map.insert(std::make_pair(ShaderResourceType::Standard, std::make_shared<Shader>("Standard_Shader")));
	auto result = shader_iter.second;
	assert(result);
	if (!result)
		return;

	shader_iter.first->second->AddAndCreateShader<VertexShader>("Shader/TextureShader.fx", "VS", "vs_5_0");
	shader_iter.first->second->AddAndCreateShader<PixelShader>("Shader/TextureShader.fx", "PS", "ps_5_0");
}

const std::shared_ptr<Shader>& ResourceManager::GetShaderResource(const ShaderResourceType& shader_type)
{
	auto shader_iter = this->m_p_shader_map.find(shader_type);

	if (shader_iter == this->m_p_shader_map.end())
		return nullptr;

	return shader_iter->second;
}

void ResourceManager::CreateMaterial()
{
	//Create Standard Material
	auto material_iter = this->m_p_material_map.insert(std::make_pair("Standard_Material", std::make_shared<Material>("Standard_Material")));
	auto result = material_iter.second;
	assert(result);
	if (!result)
		return;

	auto material = material_iter.first->second;
	material->SetShader(GetShaderResource(ShaderResourceType::Standard));
}

//Texture/objectname/~~/
void ResourceManager::CreateAnimation(const GameObjectType& game_object_type, const std::string& animation_name, const std::string& directory_path)
{
	//Create Standard Shader
	auto animation_iter = this->m_p_animation_multimap.insert(std::make_pair(game_object_type, std::make_shared<Animation>(animation_name)));

	if (animation_iter->second != nullptr)
	{
		auto result = animation_iter->second->LoadFromFile(directory_path);
		assert(result);
	}
}

//Texture/objectname/~~/textureName.확장자
const std::shared_ptr<Texture>& ResourceManager::CreateTexture(const std::string& texture_path)
{
	auto file_name = FileManager::GetIntactFileNameFromPath(texture_path);
	//확장자가 포함된 파일 이름에서 확장자 부분을 뺀 string을 해당 텍스처의 리소스 이름으로 설정
	auto texture_iter = this->m_p_textrue_map.insert(std::make_pair(file_name, std::make_shared<Texture>(file_name)));
	auto result = texture_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	auto texture = texture_iter.first->second;
	result = texture->LoadFromFile(texture_path);
	assert(result);
	if (!result)
		return nullptr;

	return texture;
}



const std::shared_ptr<Mesh>& ResourceManager::CreateMesh(const Vector2& mesh_size)
{
	auto width = static_cast<UINT>(mesh_size.x);
	auto height = static_cast<UINT>(mesh_size.y);

	//Create Standard Shader
	//매쉬의 크기값으로 저장하고 리소스 이름으로 설정
	auto mesh_iter = this->m_p_mesh_map.insert
	(
		std::make_pair(std::make_pair(width, height), std::make_shared<Mesh>(std::to_string(width) + "x" + std::to_string(height) + "_Mesh"))
	);
	auto result = mesh_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	auto mesh = mesh_iter.first->second;
	mesh->Create(MeshType::Rectangle, mesh_size);

	return mesh;
}
