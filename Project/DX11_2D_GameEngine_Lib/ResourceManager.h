#pragma once

#include "IResource.h"

class Mesh;
class Material;
class Shader;
class Texture;
class Animation;

//TODO: Object 설계 후 사용할지 결정
class ResourceManager final : public Singleton<ResourceManager>
{
private:
	template<typename T>
	static constexpr ResourceType GetResourceType();

	SINGLETON(ResourceManager);

	ResourceManager();
	~ResourceManager();

	void Initialize();

public:
	void CreateShader();
	const std::shared_ptr<Shader>& GetShaderResource(const ShaderResourceType& shader_type);

	void CreateMaterial();
	const std::shared_ptr<Shader>& GetMaterialResource(const ShaderResourceType& shader_type);

	void CreateAnimation
	(
		const GameObjectType& game_object_type,
		const std::string& animation_name,
		const std::string& directory_path
	);

	const std::shared_ptr<Mesh>& CreateMesh(const Vector2& mesh_size);
	const std::shared_ptr<Texture>& CreateTexture(const std::string& texture_path);

private:
	std::map<std::pair<UINT, UINT>, std::shared_ptr<Mesh>> m_p_mesh_map;

	std::map<std::string, std::shared_ptr<Material>> m_p_material_map;

	std::map<std::string, std::shared_ptr<Texture>> m_p_textrue_map;

	std::map<ShaderResourceType, std::shared_ptr<Shader>> m_p_shader_map;

	std::multimap<GameObjectType, std::shared_ptr<Animation>> m_p_animation_multimap;
};

