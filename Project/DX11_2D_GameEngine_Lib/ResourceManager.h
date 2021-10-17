#pragma once

#include "IResource.h"

class Shader;
class Texture;
class Mesh;


//TODO: Object 설계 후 사용할지 결정
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
	//Texture
	const std::shared_ptr<Texture>& LoadTexture(const std::string& texture_path);
	const std::shared_ptr<Texture>& GetTexture(const std::string& texture_name);
	//Mesh
	const std::shared_ptr<Mesh>& CreateMesh(const Vector2& mesh_size);
	const std::shared_ptr<Mesh>& GetMesh(const Vector2& mesh_size);

private:
	std::map<ShaderResourceType, std::shared_ptr<IResource>> m_p_shader_map;

	std::map<std::string, std::shared_ptr<IResource>> m_p_textrue_map;

	std::map<std::pair<UINT, UINT>, std::shared_ptr<IResource>> m_p_mesh_map;
};

