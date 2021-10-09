#pragma once

#include "IResource.h"
#include "IShader.h"

//하나의 그래픽스 파이프라인의 과정을 Shader로 정의
//여러 Shader(VS, HS, DS, GS, PS)들을 조합하여 사용할 수 있기 때문
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
	//Shader 추가 후 생성
	template<typename T>
	void AddAndCreateShader
	(
		const std::string& path,			//Shader 파일 경로
		const std::string& function_name,	//Shader 파일에서 사용되는 함수이름
		const std::string& shader_version   //사용할 Shader 버전
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
	//Class T가 IShader를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return;
	}

	//타입 T에 해당하는 Shader Type 반환
	auto shader_type = GetShaderType<T>();
	//un_map에 새로운 Shader 추가
	//insert에 성공하면 pair_ib<iterator, bool> 반환
	//iterator는 map 데이터를 참조하는 반복자, bool은 map에 데이터 삽입 성공여부
	auto shader_iter = m_shader_map.insert(std::make_pair(shader_type, std::make_shared<T>()));

	//해당하는 Type의 map에 성공적으로 데이터가 추가되었다면
	assert(shader_iter.second);
	if (shader_iter.second)
	{
		auto shader = std::dynamic_pointer_cast<T>(shader_iter.first->second);

		//타입 T에 해당하는 Shader 생성
		shader->Create(path, function_name, shader_version);
	}
}

template<typename T>
std::shared_ptr<T> Shader::GetShader() const
{
	//Class T가 IShader를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return nullptr;
	}

	//타입 T에 해당하는 Shader Type 반환
	auto shader_type = GetShaderType<T>();
	auto shader_iter = this->m_shader_map.find(shader_type);

	//해당 Shader Type의 Shader 데이터가 존재하는 경우
	assert(shader_iter != this->m_shader_map.end());
	//타입 T에 해당하는 Shader가 존재하는 경우
	if (shader_iter->second != nullptr)
	{
		return std::dynamic_pointer_cast<T>(shader_iter->second);
	}

	return nullptr;
}
