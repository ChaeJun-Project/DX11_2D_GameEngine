#include "stdafx.h"
#include "Shader.h"

Shader::Shader(const std::string resource_name)
	:IResource(ResourceType::Shader, resource_name)
{
}

Shader::~Shader()
{
	for (auto& shader : m_shader_un_map)
	{
		shader.second.reset();
	}

	m_shader_un_map.clear();
}

void Shader::BindPipeline()
{
    auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	auto vertex_shader = GetShader<VertexShader>();
	auto pixel_shader = GetShader<PixelShader>();
	
	device_context->VSSetShader(vertex_shader->GetVertexShader(), 0, 0);
	device_context->PSSetShader(pixel_shader->GetPixelShader(), 0, 0);

	auto input_layout = vertex_shader->GetInputLayoutClass()->GetInputLayout();
	device_context->IASetInputLayout(input_layout);
	device_context->IASetPrimitiveTopology(m_primitive_topology);
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