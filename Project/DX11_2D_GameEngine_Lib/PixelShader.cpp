#include "stdafx.h"
#include "PixelShader.h"

PixelShader::PixelShader()
    : IShader(ShaderType::PS)
{

}

void PixelShader::Create(const std::string& path, const std::string& function_name, const std::string& shader_version)
{
    m_path = path;
    m_function_name = function_name;
    m_shader_version = shader_version;

    //Pixel Shader Compile
    auto result = CompileShader(m_path, m_function_name, m_shader_version, m_p_blob.GetAddressOf());
    assert(result);
    if (!result)
        return;

    //Pixel Shader Compile ������ ���
    //ID3DBlob�� ������ ���� �������� Pixel Shader ����
    auto device = GRAPHICS_MANAGER->GetDevice();
    auto hResult = device->CreatePixelShader
    (
        m_p_blob->GetBufferPointer(),
        m_p_blob->GetBufferSize(),
        nullptr,
        m_p_pixel_shader.GetAddressOf()
    );
    assert(SUCCEEDED(hResult));
}
