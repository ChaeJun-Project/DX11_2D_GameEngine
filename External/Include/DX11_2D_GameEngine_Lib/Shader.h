#pragma once

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

//�ϳ��� �׷��Ƚ� ������������ ������ Shader�� ����
//���� Shader(VS, HS, DS, GS, PS)���� �����Ͽ� ����� �� �ֱ� ����
class Shader
{
public:
	Shader() = default;
	~Shader();

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
    std::unordered_map<ShaderType, std::shared_ptr<IShader>> m_shader_un_map;
};

template<typename T>
inline void Shader::AddAndCreateShader(const std::string& shader_name, const std::string& path, const std::string& function_name, const std::string& shader_version)
{
	//Class T�� IShader�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return;
	}

	//Ÿ�� T�� �ش��ϴ� Shader Type ��ȯ
	auto shader_type = GetShaderType<T>();
	//un_map�� ���ο� Shader �߰�
	//insert�� �����ϸ� pair_ib<iterator, bool> ��ȯ
	//iterator�� map �����͸� �����ϴ� �ݺ���, bool�� map�� ������ ���� ��������
	auto shader_iter = m_shader_un_map.insert(std::make_pair(shader_type, std::make_shared<T>()));

	//�ش��ϴ� Type�� map�� ���������� �����Ͱ� �߰��Ǿ��ٸ�
	assert(shader_iter.second);
	if (shader_iter.second)
	{
		auto shader = std::dynamic_pointer_cast<T>(shader_iter.first->second);

		//Ÿ�� T�� �ش��ϴ� Shader ����
		shader->Create(path, function_name, shader_version);
	}
}

template<typename T>
inline std::shared_ptr<T> Shader::GetShader(const std::string& shader_name) const
{
	//Class T�� IShader�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return;
	}

	//Ÿ�� T�� �ش��ϴ� Shader Type ��ȯ
	auto shader_type = GetShaderType<T>();
	auto shader_iter = this->m_shader_un_map.find(shader_type);

	assert(shader_iter);
	//Ÿ�� T�� �ش��ϴ� Shader�� �����ϴ� ���
	if (shader_iter->second != nullptr)
	{
		return std::dynamic_pointer_cast<T>(shader_iter->second);
	}

	return nullptr;
}
