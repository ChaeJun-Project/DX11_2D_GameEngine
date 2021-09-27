#pragma once

#include"Singleton.h"

#include "IShader.h"

enum class ShaderType : UINT
{
	NONE = 0,
	VS = 1, //VertexShader
	HS = 2, //Hull Shader
	DS = 3, //Domain Shader
	GS = 4, //Geometry Shader
	PS = 5, //Pixel Shader
};

//��� ������ Shader�� ������ Ŭ����
class ShaderManager final : public Singleton<ShaderManager>
{
	SINGLETON(ShaderManager);

	ShaderManager() = default;
	~ShaderManager();

public:
	template<typename T>
	static constexpr ShaderType GetShaderType();

public:
	//Shader �߰� �� ����
	template<typename T>
	void AddAndCreateShader
	(
		const std::string& shader_name,     //Shader �̸� 
		const std::string& path,			//Shader ���� ���
		const std::string& function_name,	//Shader ���Ͽ��� ���Ǵ� �Լ��̸�
		const std::string& shader_version   //����� Shader ����
	);

	template<typename T>
	std::shared_ptr<T> GetShader(const std::string& shader_name) const;

private:
	std::map<std::pair<ShaderType, std::string>, std::shared_ptr<IShader>> m_shader_map;
};


template<typename T>
inline void ShaderManager::AddAndCreateShader(const std::string& shader_name, const std::string& path, const std::string& function_name, const std::string& shader_version)
{
	//Class T�� IShader�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return;
	}

	auto shader_type = GetShaderType<T>();;
	auto shader_iter = m_shader_map.insert(std::make_pair(std::make_pair(shader_type, shader_name), std::make_shared<T>()));

	//�ش��ϴ� Type�� map�� ���������� �����Ͱ� �߰��Ǿ��ٸ�
	if (shader_iter.second)
	{
		auto shader = std::static_pointer_cast<T>(shader_iter.first->second);

		shader->Create(path, function_name, shader_version);
	}
}

template<typename T>
inline std::shared_ptr<T> ShaderManager::GetShader(const std::string& shader_name) const
{
	//Class T�� IShader�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return nullptr;
	}

	auto shader_type = GetShaderType<T>();
	auto shader_iter = this->m_shader_map.find(std::make_pair(shader_type, shader_name));

	if (shader_iter->second != nullptr)
	{
		return std::dynamic_pointer_cast<T>(shader_iter->second);
	}

	//�ش� key���� ���� Shader�� ���� ���
	return nullptr;
}
