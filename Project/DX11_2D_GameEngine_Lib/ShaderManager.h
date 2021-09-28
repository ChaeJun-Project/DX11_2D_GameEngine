#pragma once

class Shader;

enum class ShaderResourceType : UINT
{
//TODO
};


//모든 종류의 Shader를 관리할 클래스
class ShaderManager final : public Singleton<ShaderManager>
{
	SINGLETON(ShaderManager);

	ShaderManager() = default;
	~ShaderManager();



public:
	

private:
	std::map<ShaderResourceType, std::shared_ptr<Shader>> m_shader_map;
};
