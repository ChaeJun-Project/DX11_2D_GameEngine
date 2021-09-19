#include "stdafx.h"
#include "VertexShader.h"

void VertexShader::Create(const std::string& path, const std::string& function_name, const std::string& shader_version)
{
     this->m_path = path;
     this->m_function_name = function_name;
     this->m_shader_version = shader_version;

    //Vertex Shader Compile
    auto result = CompileShader(this->m_path, this->m_function_name, this->m_shader_version, this->m_p_blob.GetAddressOf());
    assert(result);
    if (!result)
        return;

    //Vertex Shader Compile 성공한 경우
    //ID3DBlob의 데이터 값을 바탕으로 Vertex Shader 생성
    auto device = GET_DEVICE;
    auto hResult = device->CreateVertexShader
    (
        this->m_p_blob->GetBufferPointer(),
        this->m_p_blob->GetBufferSize(),
        nullptr,
        this->m_p_vertex_shader.GetAddressOf()
    );
    assert(SUCCEEDED(hResult));
}