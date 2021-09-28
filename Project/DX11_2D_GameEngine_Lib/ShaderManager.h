#pragma once

class Shader;

enum class ShaderResourceType : UINT
{
//TODO
};


//��� ������ Shader�� ������ Ŭ����
class ShaderManager final : public Singleton<ShaderManager>
{
	SINGLETON(ShaderManager);

	ShaderManager() = default;
	~ShaderManager();



public:
	

private:
	std::map<ShaderResourceType, std::shared_ptr<Shader>> m_shader_map;
};
