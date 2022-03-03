#include "stdafx.h"
#include "GeometryShader.h"

GeometryShader::GeometryShader()
	:IShader(ShaderType::GS)
{
}

void GeometryShader::Create(const std::string& path, const std::string& function_name, const std::string& shader_version)
{
    m_path = path;
    m_function_name = function_name;
    m_shader_version = shader_version;

    //Geometry Shader Compile
    auto result = CompileShader(m_path, m_function_name, m_shader_version, m_p_blob.GetAddressOf());
    assert(result);
    if (!result)
        return;

    //Geometry Shader Compile ������ ���
    //ID3DBlob�� ������ ���� �������� Geometry Shader ����
    auto device = GRAPHICS_MANAGER->GetDevice();
    auto hResult = device->CreateGeometryShader
    (
        m_p_blob->GetBufferPointer(),
        m_p_blob->GetBufferSize(),
        nullptr,
        m_p_geometry_shader.GetAddressOf()
    );
    assert(SUCCEEDED(hResult));
}
