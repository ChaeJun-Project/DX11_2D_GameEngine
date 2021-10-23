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

	ResourceManager();
	~ResourceManager();

	void Initialize();

public:
    //Shader
	void CreateShader();
	const std::shared_ptr<Shader>& GetShaderResource(const ShaderResourceType& shader_type);
	//Material
	void CreateMaterial();
	const std::shared_ptr<Material>& GetMaterialResource(const std::string& material_name);
	//Texture
	const std::shared_ptr<Texture>& LoadTexture(const std::string& texture_path);
	const std::shared_ptr<Texture>& GetTexture(const std::string& texture_name);
	//Mesh
	const std::shared_ptr<Mesh>& CreateMesh(const Vector2& mesh_size);
	const std::shared_ptr<Mesh>& GetMesh(const Vector2& mesh_size);
	//Prefab
	void AddPrefab(const std::string& prefab_object_name, GameObject* p_game_object);
	const std::shared_ptr<Prefab>& GetPrefab(const std::string& game_object_name);

private:
    //Shader
	std::map<ShaderResourceType, std::shared_ptr<IResource>> m_p_shader_map;
	//Material
	std::map<std::string, std::shared_ptr<IResource>> m_p_material_map;
	//Texture
	std::map<std::string, std::shared_ptr<IResource>> m_p_textrue_map;
	//Mesh
	std::map<std::pair<UINT, UINT>, std::shared_ptr<IResource>> m_p_mesh_map;
	//Prefab
	std::map<std::string, std::shared_ptr<IResource>> m_p_prefab_map;
};

