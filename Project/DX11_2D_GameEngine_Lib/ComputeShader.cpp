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
	this->m_path = path;
	this->m_function_name = function_name;
	this->m_shader_version = shader_version;

	//Compute Shader Compile
	auto result = CompileShader(this->m_path, this->m_function_name, this->m_shader_version, this->m_p_blob.GetAddressOf());
	assert(result);
	if (!result)
		return;

	//Compute Shader Compile ������ ���
	//ID3DBlob�� ������ ���� �������� Compute Shader ����
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