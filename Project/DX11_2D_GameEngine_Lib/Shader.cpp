#include "stdafx.h"
#include "Shader.h"

#include "GraphicsManager.h"

#include "InputLayout.h"

#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

#include "ComputeShader.h"

template<typename T>
constexpr ShaderType Shader::GetShaderType()
{
	return ShaderType::NONE;
}

#define REGISTER_SHADER_TYPE(T, shader_type) template<>  ShaderType Shader::GetShaderType<T>() { return shader_type; }

//TODO: 추후에 다른 Shader들도 추가 예정
REGISTER_SHADER_TYPE(VertexShader, ShaderType::VS);
REGISTER_SHADER_TYPE(GeometryShader, ShaderType::GS);
REGISTER_SHADER_TYPE(PixelShader, ShaderType::PS);

REGISTER_SHADER_TYPE(ComputeShader, ShaderType::CS);

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

//<summary>
//Compute Shader Setting을 여기서 하지 않고 Compute Shader의 Dispatch 함수를 호출할 때 먼저 호출하도록 설계
//이유: ImGui의 NewFrame 호출 후 ComputeShader가 설정되어있지 않아 CS를 사용하는 렌더링에서 오류 발생
//</summary>
void Shader::BindPipeline()
{
	auto graphics_manager = GraphicsManager::GetInstance();

	auto device_context = graphics_manager->GetDeviceContext();

	//Vertex Shader Stage
	if (m_shader_bind_stage & PipelineStage::VS)
	{
		auto vertex_shader = GetShader<VertexShader>();
		device_context->VSSetShader(vertex_shader->GetVertexShader(), 0, 0);

		auto input_layout = vertex_shader->GetInputLayoutClass()->GetInputLayout();
		device_context->IASetInputLayout(input_layout);
		device_context->IASetPrimitiveTopology(m_primitive_topology);
	}

	//Hull Shader Stage
	if (m_shader_bind_stage & PipelineStage::HS)
	{
		//TODO
	}
	else
	{

	}

	//Domain Shader Stage
	if (m_shader_bind_stage & PipelineStage::DS)
	{
		//TODO
	}
	else
	{
		device_context->GSSetShader(nullptr, 0, 0);
	}

	//Geometry Shader Stage
	if (m_shader_bind_stage & PipelineStage::GS)
	{
		auto geometry_shader = GetShader<GeometryShader>();
		device_context->GSSetShader(geometry_shader->GetGeometryShader(), 0, 0);
	}
	else
	{
		device_context->GSSetShader(nullptr, 0, 0);
	}

	//Pixel Shader Stage
	if (m_shader_bind_stage & PipelineStage::PS)
	{
		auto pixel_shader = GetShader<PixelShader>();
		device_context->PSSetShader(pixel_shader->GetPixelShader(), 0, 0);
	}

	//Rasterizer 적용
	auto rasterizer = graphics_manager->GetRasterizer(m_rasterizer_type);
	device_context->RSSetState(rasterizer->GetRasterizerState());

	//DepthStencil 적용
	auto depth_stencil = graphics_manager->GetDepthStencilState(m_depth_stencil_type);
	device_context->OMSetDepthStencilState(depth_stencil->GetDepthStencilState(), 0);

	//Blender 적용
	auto blender = graphics_manager->GetBlender(m_blend_type);
	device_context->OMSetBlendState(blender->GetBlendState(), nullptr, 0xFF);
}