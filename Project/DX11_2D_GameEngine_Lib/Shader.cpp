#include "stdafx.h"
#include "Shader.h"

template<typename T>
constexpr ShaderType Shader::GetShaderType()
{
	return ShaderType::NONE;
}

#define REGISTER_SHADER_TYPE(T, shader_type) template<>  ShaderType Shader::GetShaderType<T>() { return shader_type; }

REGISTER_SHADER_TYPE(VertexShader, ShaderType::VS);
REGISTER_SHADER_TYPE(PixelShader, ShaderType::PS);
//TODO: 추후에 다른 Shader들도 추가 예정

Shader::Shader(const std::string resource_name)
	:IResource(ResourceType::Shader, resource_name)
{
}

Shader::~Shader()
{
	for (auto& shader : m_shader_map)
	{
		shader.second.reset();
	}

	m_shader_map.clear();
}

void Shader::BindPipeline()
{
    static auto graphics_manager = GraphicsManager::GetInstance();

	auto device_context = graphics_manager->GetDeviceContext();
	auto vertex_shader = GetShader<VertexShader>();
	auto pixel_shader = GetShader<PixelShader>();
	
	device_context->VSSetShader(vertex_shader->GetVertexShader(), 0, 0);
	device_context->PSSetShader(pixel_shader->GetPixelShader(), 0, 0);

	auto input_layout = vertex_shader->GetInputLayoutClass()->GetInputLayout();
	device_context->IASetInputLayout(input_layout);
	device_context->IASetPrimitiveTopology(m_primitive_topology);

	//Rasterizer 적용
	auto rasterizer = graphics_manager->GetRasterizer(this->m_rasterizer_type);
	device_context->RSSetState(rasterizer->GetRasterizerState());

	//DepthStencil 적용
	auto depth_stencil = graphics_manager->GetDepthStencilState(this->m_depth_stencil_type);
	device_context->OMSetDepthStencilState(depth_stencil->GetDepthStencilState(), 0);

	//Blender 적용
	auto blender = graphics_manager->GetBlender(this->m_blend_type);
	device_context->OMSetBlendState(blender->GetBlendState(), nullptr, 0xFF);
}