#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
}

template<typename T>
constexpr ShaderType ShaderManager::GetShaderType()
{
    return ShaderType::NONE;
}

#define REGISTER_SHADER_TYPE(T, shader_type) template<>  ShaderType ShaderManager::GetShaderType<T>() { return shader_type; }

REGISTER_SHADER_TYPE(VertexShader, ShaderType::VS);
REGISTER_SHADER_TYPE(PixelShader, ShaderType::PS);
//TODO: 추후에 다른 Shader들도 추가 예정