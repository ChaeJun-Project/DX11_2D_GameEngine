#pragma once

#include "IResource.h"

class Shader;
class ComputeShader;

class Material;
class Texture;
class Mesh;
class AudioClip;
class Prefab;
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
	const std::shared_ptr<T>& GetResource(const std::string& resource_name);

	const ResourceMap& GetResourceMap(const ResourceType& resource_type);

	template<typename T>
	void SaveResource(std::shared_ptr<T> p_resource, FILE* p_file);

	template<typename T>
	void LoadResource(std::shared_ptr<T>& p_resource, FILE* p_file);

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
	void CreateDefaultMaterial();
	const std::shared_ptr<Material>& CreateMaterial(const std::string& material_name, const std::string& shader_name);
	
	//Texture
	void CreateDefaultTexture();
	const std::shared_ptr<Texture>& CreateTexture(const std::string& texture_path);
	const std::shared_ptr<Texture>& CreateTexture(const std::string& texture_name, const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage);
	const std::shared_ptr<Texture>& CreateTexture(const std::string& texture_name, const ComPtr<ID3D11Texture2D>& texture2D);

	//Mesh
	void CreateDefaultMesh();
	const std::shared_ptr<Mesh>& CreateMesh(const std::string& mesh_name, const MeshType& mesh_type);
	
	//Audio
	const std::shared_ptr<AudioClip>& CreateAudioClip(const std::string& audio_clip_path);
	
	//Prefab
	const std::shared_ptr<Prefab>& CreatePrefab(GameObject* p_game_object);
	
private:
	//<summary>
	//ResourceMap == std::map<std::string, std::shared_ptr<IResource>>
	//</summary>
	std::map<ResourceType, ResourceMap> m_resources_map;
	std::map<std::string, std::shared_ptr<ComputeShader>> m_p_compute_shader_map;
};

template<typename T>
const std::shared_ptr<T>& ResourceManager::GetResource(const std::string& resource_name)
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

#ifdef _DEBUG
	if (resource_iter == resource_map.end())
		return nullptr;
#endif 

	return std::dynamic_pointer_cast<T>(resource_iter->second);
}

template<typename T>
inline void ResourceManager::SaveResource(std::shared_ptr<T> p_resource, FILE* p_file)
{
}

template<typename T>
inline void ResourceManager::LoadResource(std::shared_ptr<T>& p_resource, FILE* p_file)
{
}
