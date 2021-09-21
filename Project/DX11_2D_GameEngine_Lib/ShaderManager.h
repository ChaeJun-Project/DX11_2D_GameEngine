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
	template<typename T>
	std::shared_ptr<T> GetShader(const std::string& shader_name) const;

	template<typename T>
	void AddShader
	(
		const std::string& shader_name,     //Shader �̸� 
		const std::string& path,			//Shader ���� ���
		const std::string& function_name,	//Shader ���Ͽ��� ���Ǵ� �Լ��̸�
		const std::string& shader_version   //����� Shader ����
	);

private:

	std::map<std::string, std::shared_ptr<IShader> > m_vertex_shader_map;
	//std::map<std::string, std::shared_ptr<IShader> > m_hull_shader_map;
	//std::map<std::string, std::shared_ptr<IShader> > m_domain_shader_map;
	//std::map<std::string, std::shared_ptr<IShader> > m_geometry_shader_map;
	std::map<std::string, std::shared_ptr<IShader> > m_pixel_shader_map;
};

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
	
	switch (shader_type)
	{
	case ShaderType::VS:
		if (this->m_vertex_shader_map.find(shader_name) != this->m_vertex_shader_map.end())
			return dynamic_cast<T>(this->m_vertex_shader_map[shader_name]);
		break;
	case ShaderType::HS:

		break;
	case ShaderType::DS:

		break;
	case ShaderType::GS:

		break;
	case ShaderType::PS:
		if (this->m_pixel_shader_map.find(shader_name) != this->m_pixel_shader_map.end())
			return dynamic_cast<T>(this->m_pixel_shader_map[shader_name]);
		break;
	}

	//�ش� key���� ���� Shader�� ���� ���
	return nullptr;
}

template<typename T>
inline void ShaderManager::AddShader(const std::string& shader_name, const std::string& path, const std::string& function_name, const std::string& shader_version)
{
	//Class T�� IShader�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return;
	}

	auto shader_type = GetShaderType<T>();
	//TODO
	
}
