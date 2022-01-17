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

	template<typename T>
	const std::shared_ptr<T>& LoadFromFile(const std::string& resource_path);

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
	//Class T�� IResource�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IResource, T>::value;
	assert(result);
#ifdef _DEBUG
	if (!result)
		return nullptr;
#endif 

	auto resource_type = GetResourceType<T>(); //Ÿ�� T�� �ش��ϴ� Resource Type ��ȯ
	auto resource_map = m_resources_map[resource_type]; //Resource Type�� �ش��ϴ� Resource Map ��ȯ

	auto resource_iter = resource_map.find(resource_name);

#ifdef _DEBUG
	if (resource_iter == resource_map.end())
		return nullptr;
#endif 

	return std::dynamic_pointer_cast<T>(resource_iter->second);
}

template<typename T>
void ResourceManager::SaveResource(std::shared_ptr<T> p_resource, FILE* p_file)
{
	//���ҽ��� �Ҵ�� ������ ���ٸ�
	if (p_resource == nullptr)
	{
		fprintf(p_file, "[Resource Name]\n");
		fprintf(p_file, "None\n");

		fprintf(p_file, "[Resource Path]\n");
		fprintf(p_file, "None\n");
		return;
	}


	//Class T�� IResource�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IResource, T>::value;
	assert(result);
#ifdef _DEBUG
	if (!result)
		return;
#endif 

	fprintf(p_file, "[Resource Name]\n");
	fprintf(p_file, "%s\n", p_resource->GetResourceName().c_str());

	fprintf(p_file, "[Resource Path]\n");
	fprintf(p_file, "%s\n", p_resource->GetResourcePath().c_str());
}

template<typename T>
void ResourceManager::LoadResource(std::shared_ptr<T>& p_resource, FILE* p_file)
{
	//Class T�� IResource�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IResource, T>::value;
	assert(result);
#ifdef _DEBUG
	if (!result)
		return;
#endif 

	char char_buffer[256] = {};

	//Resource Name
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf(char_buffer, p_file);

	//Resource Path
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf(char_buffer, p_file);
	std::string resource_path = std::string(char_buffer);

	if (resource_path._Equal("None"))
		return;

	if (std::is_same<T, Material>::value || std::is_same<T, AudioClip>::value)
	{
		auto clone_resource = LoadFromFile<T>(resource_path)->Clone();
		p_resource = std::shared_ptr<T>(clone_resource);
	}
	
	else
		p_resource = LoadFromFile<T>(resource_path);
}

template<typename T>
const std::shared_ptr<T>& ResourceManager::LoadFromFile(const std::string& resource_path)
{
	//Class T�� IResource�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IResource, T>::value;
	assert(result);
	if (!result)
		return nullptr;

	auto resource_type = GetResourceType<T>(); //Ÿ�� T�� �ش��ϴ� Resource Type ��ȯ
	auto& resource_map = m_resources_map[resource_type]; //Resource Type�� �ش��ϴ� Resource Map ��ȯ
	auto resource_name = FileManager::GetOriginFileNameFromPath(resource_path);

	//�ش� ���ҽ��� �̹� �����Ѵٸ�
	auto resource_iter = resource_map.find(resource_name);
	if (resource_iter != resource_map.end())
		return std::dynamic_pointer_cast<T>(resource_iter->second);

	//�������� ������ ���ο� ���ҽ� ����
	auto p_resource = std::make_shared<T>(resource_name);
	p_resource->SetResourcePath(resource_path);

	//LoadFromFile
	if (!p_resource->LoadFromFile(resource_path))
		return nullptr;

	//�ش� ���ҽ� Map�� ���� ������ ���ҽ� �߰�
	auto resource_pair_iter = resource_map.insert(std::make_pair(resource_name, p_resource));
	result = resource_pair_iter.second;
	assert(result);
	if (!result)
		return nullptr;

	//�߰��� ���ҽ��� ���� ������ ��ȯ
	return std::dynamic_pointer_cast<T>(resource_pair_iter.first->second);
}
