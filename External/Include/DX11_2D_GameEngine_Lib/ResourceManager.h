#pragma once

#include "IResource.h"
#include "SceneManager.h"

class Shader;
class ComputeShader;

class Material;
class Texture;
class Mesh;
class AudioClip;
class Prefab;
class SpriteAnimation;
class Particle;
class TileMap;

class GameObject;

typedef std::map<std::string, std::shared_ptr<IResource>> ResourceMap;

class ResourceManager final : public Singleton<ResourceManager>
{
	SINGLETON(ResourceManager);

	ResourceManager() = default;
	~ResourceManager();

private:
	template<typename T>
	static constexpr ResourceType GetResourceType();

public:
	template<typename T>
	const std::shared_ptr<T> GetResource(const std::string& resource_name);

	const ResourceMap GetResourceMap(const ResourceType& resource_type);

	template<typename T>
	void SaveResource(const std::shared_ptr<T>& p_resource, FILE* p_file);

	template<typename T>
	void SaveToFile(const std::shared_ptr<T>& p_resource, const std::string& resource_path);

	template<typename T>
	void LoadResource(std::shared_ptr<T>& p_resource, FILE* p_file);

	template<typename T>
	const std::shared_ptr<T> LoadFromFile(const std::string& resource_path);

public:
	void Initialize();

private:
	void CreateResourceMap();

public:
	//Shader
	void CreateDefaultShader();

	//Compute Shader
	void AddComputeShader(const std::string& compute_shader_name, const std::shared_ptr<ComputeShader>& p_compute_shader);
	const std::shared_ptr<ComputeShader>& GetComputeShader(const std::string& compute_shader_name);

	//Material
	void CreateAddedMaterial();
	const std::shared_ptr<Material> CreateMaterial(const std::string& material_name, const std::string& shader_name);

	//Texture
	void CreateDefaultTexture();
	const std::shared_ptr<Texture> CreateIconTexture(const std::string& icon_texture_path);
	const std::shared_ptr<Texture> CreateFileItemThumbnailTexture(const std::string& file_path);
	const std::shared_ptr<Texture> CreateTexture(const std::string& texture_name, const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage);
	const std::shared_ptr<Texture> CreateTexture(const std::string& texture_name, const ComPtr<ID3D11Texture2D>& texture2D);

	//Mesh
	void CreateAddedMesh();
	const std::shared_ptr<Mesh> CreateMesh(const std::string& mesh_name, const MeshType& mesh_type);

	//Prefab
	void CreateAddedPrefab();
	const std::shared_ptr<Prefab> CreatePrefab(GameObject* p_game_object);

	//SpriteAnimation
	const std::shared_ptr<SpriteAnimation> CreateSpriteAnimation(const std::string& animation2D_name);

	//Particle
	void CreateAddedParticle();
	const std::shared_ptr<Particle> CreateParticle(const std::string& particle_name);

	//TileMap
	const std::shared_ptr<TileMap> CreateTileMap(const std::string& tile_map_name);

private:
	//<summary>
	//ResourceMap == std::map<std::string, std::shared_ptr<IResource>>
	//</summary>
	std::map<ResourceType, ResourceMap> m_resources_map;
	std::map<std::string, std::shared_ptr<ComputeShader>> m_p_compute_shader_map;
};

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
template<typename T>
const std::shared_ptr<T> ResourceManager::GetResource(const std::string& resource_name)
{
	//Class T가 IResource를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IResource, T>::value;
	assert(result);
#ifdef _DEBUG
	if (!result)
		return nullptr;
#endif 

	auto resource_type = GetResourceType<T>(); //타입 T에 해당하는 Resource Type 반환
	auto resource_map = m_resources_map[resource_type]; //Resource Type에 해당하는 Resource Map 반환

	auto resource_iter = resource_map.find(resource_name);

	if (resource_iter == resource_map.end())
		return nullptr;

	return std::dynamic_pointer_cast<T>(resource_iter->second);
}

template<typename T>
void ResourceManager::SaveResource(const std::shared_ptr<T>& p_resource, FILE* p_file)
{
	//Class T가 IResource를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IResource, T>::value;
	assert(result);
#ifdef _DEBUG
	if (!result)
		return;
#endif 

	fprintf(p_file, "[Resource Name]\n");
	if (p_resource == nullptr) 	//리소스에 할당된 정보가 없다면
		fprintf(p_file, "None\n");

	else
		fprintf(p_file, "%s\n", p_resource->GetResourceName().c_str());

	fprintf(p_file, "[Resource Path]\n");
	if (p_resource == nullptr) 	//리소스에 할당된 정보가 없다면
		fprintf(p_file, "None\n");

	else
		fprintf(p_file, "%s\n", p_resource->GetResourcePath().c_str());

	//해당 리소스가 SpriteAnimation, TileMap, Particle인 경우
	if (std::is_same<T, SpriteAnimation>::value || std::is_same<T, TileMap>::value || std::is_same<T, Particle>::value)
	{
		std::string resource_path = p_resource->GetResourcePath(); // Asset/...
		auto absolute_content_path = ABSOLUTE_CONTENT_PATH;
		resource_path = absolute_content_path + resource_path;
		SaveToFile<T>(p_resource, resource_path);
	}
}

template<typename T>
void ResourceManager::SaveToFile(const std::shared_ptr<T>& p_resource, const std::string& resource_path)
{
	//Class T가 IResource를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IResource, T>::value;
	assert(result);
#ifdef _DEBUG
	if (!result)
		return;
#endif 

	//resource_path가 상대경로로 들어왔을 경우
	auto absolute_resource_path = resource_path;
	if (absolute_resource_path.find("Content") == std::string::npos)
	{
		auto absolute_content_path = ABSOLUTE_CONTENT_PATH;
		absolute_resource_path = absolute_content_path + absolute_resource_path;
	}

	auto file_name = FILE_MANAGER->GetFileNameFromPath(absolute_resource_path);

	//SaveToFile
	if (!p_resource->SaveToFile(absolute_resource_path))
	{
		LOG_ERROR_F("리소스 파일 저장에 실패했습니다: [%s]", file_name.c_str());
		return;
	}

	LOG_INFO_F("리소스 파일 저장에 성공했습니다: [%s]", file_name.c_str());
}

template<typename T>
void ResourceManager::LoadResource(std::shared_ptr<T>& p_resource, FILE* p_file)
{
	//Class T가 IResource를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IResource, T>::value;
	assert(result);
#ifdef _DEBUG
	if (!result)
		return;
#endif 

	char char_buffer[256] = {};

	//Resource Name
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);

	//Resource Path
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);
	std::string resource_path = std::string(char_buffer);

	if (resource_path._Equal("None"))
		return;

	if (std::is_same<T, Material>::value || std::is_same<T, SpriteAnimation>::value || std::is_same<T, Particle>::value)
	{
		auto clone_resource = LoadFromFile<T>(resource_path)->Clone();
		p_resource = std::shared_ptr<T>(clone_resource);
	}

	else
	{
		p_resource = LoadFromFile<T>(resource_path);
	}
}

template<typename T>
const std::shared_ptr<T> ResourceManager::LoadFromFile(const std::string& resource_path)
{
	//Class T가 IResource를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IResource, T>::value;
	assert(result);
	if (!result)
		return nullptr;

	auto resource_type = GetResourceType<T>(); //타입 T에 해당하는 Resource Type 반환
	auto& resource_map = m_resources_map[resource_type]; //Resource Type에 해당하는 Resource Map 반환
	auto resource_name = FILE_MANAGER->GetOriginFileNameFromPath(resource_path);
	auto file_name = FILE_MANAGER->GetFileNameFromPath(resource_path);

	//해당 리소스가 이미 존재한다면
	auto resource_iter = resource_map.find(resource_name);
	if (resource_iter != resource_map.end())
		return std::dynamic_pointer_cast<T>(resource_iter->second);

	auto p_resource = std::make_shared<T>(resource_name);

	auto absolute_resource_path = resource_path;
	//resource_path가 상대경로로 들어왔을 경우
	if (absolute_resource_path.find("Content") == std::string::npos)
	{
		p_resource->SetResourcePath(resource_path);

		auto absolute_content_path = ABSOLUTE_CONTENT_PATH;
		absolute_resource_path = absolute_content_path + absolute_resource_path;
	}

	//resource_path가 절대경로로 들어왔을 경우
	else
		p_resource->SetResourcePath(FILE_MANAGER->GetRelativeResourcePathFromAbsolutePath(resource_path));

	//LoadFromFile
	if (!p_resource->LoadFromFile(absolute_resource_path))
	{
		LOG_ERROR_F("리소스 파일 로드에 실패했습니다: [%s]", file_name.c_str());
		return nullptr;
	}

	LOG_INFO_F("리소스 파일 로드에 성공했습니다: [%s] ", file_name.c_str());

	//해당 리소스 Map에 새로 생성한 리소스 추가
	auto resource_pair_iter = resource_map.insert(std::make_pair(resource_name, p_resource));
	result = resource_pair_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	//추가된 리소스의 복제 포인터 반환
	return std::dynamic_pointer_cast<T>(resource_pair_iter.first->second);
}
