#include "stdafx.h"
#include "Shader.h"

Shader::~Shader()
{
	for (auto& shader : m_shader_un_map)
	{
		shader.second.reset();
	}

	m_shader_un_map.clear();
}

template<typename T>
constexpr ShaderType Shader::GetShaderType()
{
	return ShaderType::NONE;
}

#define REGISTER_SHADER_TYPE(T, shader_type) template<>  ShaderType Shader::GetShaderType<T>() { return shader_type; }

REGISTER_SHADER_TYPE(VertexShader, ShaderType::VS);
REGISTER_SHADER_TYPE(PixelShader, ShaderType::PS);
//TODO: 추후에 다른 Shader들도 추가 예정