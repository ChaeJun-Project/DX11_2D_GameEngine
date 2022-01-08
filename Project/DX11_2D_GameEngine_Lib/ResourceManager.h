#pragma once

#include "IResource.h"

class Shader;
class ComputeShader;

class Material;
class Texture;
class Mesh;
class Prefab;
class GameObject;

class ResourceManager final : public Singleton<ResourceManager>
{
	SINGLETON(ResourceManager);

	ResourceManager() = default;
	~ResourceManager();

public:
	void Initialize();

	template<typename T>
	void SaveResource(std::shared_ptr<T> p_resource, FILE* p_file);

	template<typename T>
	void LoadResource(std::shared_ptr<T>& p_resource, FILE* p_file);

public:
    //Shader
	std::map<std::string, std::shared_ptr<IResource>>& GetShaderMap() { return m_p_shader_map; }
	void CreateDefaultShader();
	const std::shared_ptr<Shader>& GetShader(const std::string& shader_name);
	void AddComputeShader(const std::string& compute_shader_name, const std::shared_ptr<ComputeShader>& p_compute_shader);
	const std::shared_ptr<ComputeShader>& GetComputeShader(const std::string& compute_shader_name);

	//Material
	std::map<std::string, std::shared_ptr<IResource>>& GetMaterialMap() { return m_p_material_map; }
	void CreateDefaultMaterial();
	const std::shared_ptr<Material>& GetMaterial(const std::string& material_name);
	
	//Texture
	std::map<std::string, std::shared_ptr<IResource>>& GetTextureMap() { return m_p_textrue_map; }
	void CreateDefaultTexture();
	const std::shared_ptr<Texture>& LoadTexture(const std::string& texture_path, const TextureType& texture_type);
	const std::shared_ptr<Texture>& CreateTexture(const std::string& texture_name, const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage);
	const std::shared_ptr<Texture>& CreateTexture(const std::string& texture_name, const ComPtr<ID3D11Texture2D>& texture2D);
	const std::shared_ptr<Texture>& GetTexture(const std::string& texture_name);
	//Atlas
	std::map<std::string, std::shared_ptr<IResource>>& GetAtlasTextureMap() { return m_p_atlas_textrue_map; }
	const std::shared_ptr<Texture>& GetAtlasTexture(const std::string& atlas_texture_name);
	//Tile Atlas
	std::map<std::string, std::shared_ptr<IResource>>& GetTileAtlasTextureMap() { return m_p_tile_atlas_textrue_map; }
	const std::shared_ptr<Texture>& GetTileAtlasTexture(const std::string& tile_atlas_texture_name);
	
	//Mesh
	std::map<MeshType, std::shared_ptr<IResource>>& GetMeshMap() { return m_p_mesh_map; }
	void CreateDefaultMesh();
	const std::shared_ptr<Mesh>& CreateMesh(const MeshType& mesh_type);
	const std::shared_ptr<Mesh>& GetMesh(const MeshType& mesh_type);
	
	//Prefab
	std::map<std::string, std::shared_ptr<IResource>>& GetPrefabMap() { return m_p_prefab_map; }
	void AddPrefab(const std::string& prefab_object_name, GameObject* p_game_object);
	const std::shared_ptr<Prefab>& GetPrefab(const std::string& game_object_name);

private:
    //Shader
	std::map<std::string, std::shared_ptr<IResource>> m_p_shader_map;
	std::map<std::string, std::shared_ptr<ComputeShader>> m_p_compute_shader_map;

	//Material
	std::map<std::string, std::shared_ptr<IResource>> m_p_material_map;

	//Texture
	std::map<std::string, std::shared_ptr<IResource>> m_p_textrue_map;  //Standard
	std::map<std::string, std::shared_ptr<IResource>> m_p_atlas_textrue_map; //Atlas
	std::map<std::string, std::shared_ptr<IResource>> m_p_tile_atlas_textrue_map; //Tile_Altas

	//Mesh
	std::map<MeshType, std::shared_ptr<IResource>> m_p_mesh_map;

	//Prefab
	std::map<std::string, std::shared_ptr<IResource>> m_p_prefab_map;
};

template<typename T>
inline void ResourceManager::SaveResource(std::shared_ptr<T> p_resource, FILE* p_file)
{
}

template<typename T>
inline void ResourceManager::LoadResource(std::shared_ptr<T>& p_resource, FILE* p_file)
{
}
