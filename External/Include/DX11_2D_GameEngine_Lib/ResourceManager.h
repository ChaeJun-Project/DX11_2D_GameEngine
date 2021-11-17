#pragma once

#include "IResource.h"

class Shader;
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

    //Shader
	void CreateDefaultShader();
	const std::shared_ptr<Shader>& GetShaderResource(const ShaderResourceType& shader_type, const std::string shader_name);
	//Material
	void CreateDefaultMaterial();
	const std::shared_ptr<Material>& GetMaterial(const std::string& material_name);
	//Texture
	void CreateDefaultTexture();
	const std::shared_ptr<Texture>& LoadTexture(const std::string& texture_path);
	const std::shared_ptr<Texture>& CreateTexture(const std::string& texture_name, const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage);
	const std::shared_ptr<Texture>& CreateTexture(const std::string& texture_name, const ComPtr<ID3D11Texture2D>& texture2D);
	const std::shared_ptr<Texture>& GetTexture(const std::string& texture_name);
	//Mesh
	void CreateDefaultMesh();
	const std::shared_ptr<Mesh>& CreateMesh(const MeshType& mesh_type);
	const std::shared_ptr<Mesh>& GetMesh(const MeshType& mesh_type);
	//Prefab
	void AddPrefab(const std::string& prefab_object_name, GameObject* p_game_object);
	const std::shared_ptr<Prefab>& GetPrefab(const std::string& game_object_name);

private:
    //Shader
	std::map<std::pair<ShaderResourceType, std::string>, std::shared_ptr<IResource>> m_p_shader_map;
	//Material
	std::map<std::string, std::shared_ptr<IResource>> m_p_material_map;
	//Texture
	std::map<std::string, std::shared_ptr<IResource>> m_p_textrue_map;
	//Mesh
	std::map<MeshType, std::shared_ptr<IResource>> m_p_mesh_map;
	//Prefab
	std::map<std::string, std::shared_ptr<IResource>> m_p_prefab_map;
};

