#include "stdafx.h"
#include "ComputeShader.h"

#include "GraphicsManager.h"

#include "Texture.h"

ComputeShader::ComputeShader(const UINT& thread_group_x, const UINT& thread_group_y, const UINT& thread_group_z)
	: IShader(ShaderType::CS),
	m_thread_group_x(thread_group_x),
	m_thread_group_y(thread_group_y),
	m_thread_group_z(thread_group_z)
{
}

void ComputeShader::Create(const std::string& path, const std::string& function_name, const std::string& shader_version)
{
	this->m_path = path;
	this->m_function_name = function_name;
	this->m_shader_version = shader_version;

	//Compute Shader Compile
	auto result = CompileShader(this->m_path, this->m_function_name, this->m_shader_version, this->m_p_blob.GetAddressOf());
	assert(result);
	if (!result)
		return;

	//Compute Shader Compile 성공한 경우
	//ID3DBlob의 데이터 값을 바탕으로 Compute Shader 생성
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateComputeShader
	(
		this->m_p_blob->GetBufferPointer(),
		this->m_p_blob->GetBufferSize(),
		nullptr,
		this->m_p_compute_shader.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));
}

void ComputeShader::Dispatch(const UINT& thread_group_x, const UINT& thread_group_y, const UINT& thread_group_z)
{
	BindPipeline();

	this->m_thread_group_x = thread_group_x;
	this->m_thread_group_y = thread_group_y;
	this->m_thread_group_z = thread_group_z;

	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	device_context->CSSetShader(m_p_compute_shader.Get(), nullptr, 0);
	device_context->Dispatch(this->m_thread_group_x, this->m_thread_group_y, this->m_thread_group_z);

	Clear();
}

void ComputeShader::BindPipeline()
{
	if (m_p_texture != nullptr)
	{
		m_p_texture->SetPipelineStage(PipelineStage::CS);
		m_p_texture->SetBindSlot(0);

		m_p_texture->BindPipeline();
	}
}

void ComputeShader::Clear()
{
	m_p_texture->Clear();
}
