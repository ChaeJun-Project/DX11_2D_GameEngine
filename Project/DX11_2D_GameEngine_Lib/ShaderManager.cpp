#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
	for(auto& shader : m_shader_map)
	{
		shader.second.reset();
    }

	m_shader_map.clear();
}

template<typename T>
constexpr ShaderType ShaderManager::GetShaderType()
{
	return ShaderType::NONE;
}

#define REGISTER_SHADER_TYPE(T, shader_type) template<>  ShaderType ShaderManager::GetShaderType<T>() { return shader_type; }

REGISTER_SHADER_TYPE(VertexShader, ShaderType::VS);
REGISTER_SHADER_TYPE(PixelShader, ShaderType::PS);
//TODO: ���Ŀ� �ٸ� Shader�鵵 �߰� ����