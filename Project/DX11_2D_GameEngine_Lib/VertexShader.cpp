#include "stdafx.h"
#include "VertexShader.h"
#include "InputLayout.h"

VertexShader::VertexShader()
: IShader(ShaderType::VS)
{

}

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
    auto device = GraphicsManager::GetInstance()->GetDevice();
    auto hResult = device->CreateVertexShader
    (
        this->m_p_blob->GetBufferPointer(),
        this->m_p_blob->GetBufferSize(),
        nullptr,
        this->m_p_vertex_shader.GetAddressOf()
    );
    assert(SUCCEEDED(hResult));

    //Vertex Shader 생성에 성공 후 ID3DBlob의 데이터를 바탕으로
    //InputLayout 생성
    result =  m_p_input_layout_class->Create(m_p_blob.Get());
    assert(result);
    if (!result)
        return;
}