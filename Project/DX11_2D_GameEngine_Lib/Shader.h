#pragma once

#include "IResource.h"
#include "IShader.h"

//�ϳ��� �׷��Ƚ� ������������ ������ Shader�� ����
//���� Shader(VS, HS, DS, GS, PS)���� �����Ͽ� ����� �� �ֱ� ����
class Shader final : public IResource
{
public:
	Shader(const std::string resource_name);
	~Shader();

	void BindPipeline() override;

private:
	template<typename T>
	static constexpr ShaderType GetShaderType();

public:
	//Shader �߰� �� ����
	template<typename T>
	void AddAndCreateShader
	(
		const std::string& path,			//Shader ���� ���
		const std::string& function_name,	//Shader ���Ͽ��� ���Ǵ� �Լ��̸�
		const std::string& shader_version   //����� Shader ����
	);

	template<typename T>
	std::shared_ptr<T> GetShader() const;

	const D3D11_PRIMITIVE_TOPOLOGY& GetPrimitiveTopology() const { return this->m_primitive_topology; }
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitive_topology) { this->m_primitive_topology = primitive_topology; }

private:
    std::map<ShaderType, std::shared_ptr<IShader>> m_shader_map;
	D3D11_PRIMITIVE_TOPOLOGY m_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

template<typename T>
void Shader::AddAndCreateShader(const std::string& path, const std::string& function_name, const std::string& shader_version)
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
	auto shader_iter = m_shader_map.insert(std::make_pair(shader_type, std::make_shared<T>()));

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
std::shared_ptr<T> Shader::GetShader() const
{
	//Class T�� IShader�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return nullptr;
	}

	//Ÿ�� T�� �ش��ϴ� Shader Type ��ȯ
	auto shader_type = GetShaderType<T>();
	auto shader_iter = this->m_shader_map.find(shader_type);

	//�ش� Shader Type�� Shader �����Ͱ� �����ϴ� ���
	assert(shader_iter != this->m_shader_map.end());
	//Ÿ�� T�� �ش��ϴ� Shader�� �����ϴ� ���
	if (shader_iter->second != nullptr)
	{
		return std::dynamic_pointer_cast<T>(shader_iter->second);
	}

	return nullptr;
}
