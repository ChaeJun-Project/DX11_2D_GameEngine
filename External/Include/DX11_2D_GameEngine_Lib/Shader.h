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

//하나의 그래픽스 파이프라인의 과정을 Shader로 정의
//여러 Shader(VS, HS, DS, GS, PS)들을 조합하여 사용할 수 있기 때문
class Shader
{
public:
	Shader() = default;
	~Shader();

	template<typename T>
	static constexpr ShaderType GetShaderType();

public:
	//Shader 추가 후 생성
	template<typename T>
	void AddAndCreateShader
	(
		const std::string& shader_name,     //Shader 이름 
		const std::string& path,			//Shader 파일 경로
		const std::string& function_name,	//Shader 파일에서 사용되는 함수이름
		const std::string& shader_version   //사용할 Shader 버전
	);

	template<typename T>
	std::shared_ptr<T> GetShader(const std::string& shader_name) const;

private:
    std::unordered_map<ShaderType, std::shared_ptr<IShader>> m_shader_un_map;
};

template<typename T>
inline void Shader::AddAndCreateShader(const std::string& shader_name, const std::string& path, const std::string& function_name, const std::string& shader_version)
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
	auto shader_iter = m_shader_un_map.insert(std::make_pair(shader_type, std::make_shared<T>()));

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
inline std::shared_ptr<T> Shader::GetShader(const std::string& shader_name) const
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
	auto shader_iter = this->m_shader_un_map.find(shader_type);

	assert(shader_iter);
	//타입 T에 해당하는 Shader가 존재하는 경우
	if (shader_iter->second != nullptr)
	{
		return std::dynamic_pointer_cast<T>(shader_iter->second);
	}

	return nullptr;
}
