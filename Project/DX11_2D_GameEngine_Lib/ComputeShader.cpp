#include "stdafx.h"
#include "ComputeShader.h"

#include "GraphicsManager.h"

#include "Texture.h"

ComputeShader::ComputeShader()
	: IShader(ShaderType::CS)
{

}

void ComputeShader::Create(const std::string& path, const std::string& function_name, const std::string& shader_version)
{
	m_path = path;
	m_function_name = function_name;
	m_shader_version = shader_version;

	//Compute Shader Compile
	auto result = CompileShader(m_path, m_function_name, m_shader_version, m_p_blob.GetAddressOf());
	assert(result);
	if (!result)
		return;

	//Compute Shader Compile 성공한 경우
	//ID3DBlob의 데이터 값을 바탕으로 Compute Shader 생성
	auto device = GRAPHICS_MANAGER->GetDevice();
	auto hResult = device->CreateComputeShader
	(
		m_p_blob->GetBufferPointer(),
		m_p_blob->GetBufferSize(),
		nullptr,
		m_p_compute_shader.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));
}

void ComputeShader::Dispatch(const UINT& thread_group_x_count, const UINT& thread_group_y_count, const UINT& thread_group_z_count)
{
	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();
	device_context->CSSetShader(m_p_compute_shader.Get(), 0, 0);
	device_context->Dispatch(m_thread_group_x, m_thread_group_y, m_thread_group_z);
}
