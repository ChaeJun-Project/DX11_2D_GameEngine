#include "stdafx.h"
#include "PixelShader.h"

void PixelShader::Create(const std::string& path, const std::string& function_name, const std::string& shader_version)
{
    this->m_path = path;
    this->m_function_name = function_name;
    this->m_shader_version = shader_version;

    //Pixel Shader Compile
    auto result = CompileShader(this->m_path, this->m_function_name, this->m_shader_version, this->m_p_blob.GetAddressOf());
    assert(result);
    if (!result)
        return;

    //Pixel Shader Compile 성공한 경우
    //ID3DBlob의 데이터 값을 바탕으로 Pixel Shader 생성
    auto device = GraphicsManager::GetInstance()->GetDevice();
    auto hResult = device->CreatePixelShader
    (
        this->m_p_blob->GetBufferPointer(),
        this->m_p_blob->GetBufferSize(),
        nullptr,
        this->m_p_pixel_shader.GetAddressOf()
    );
    assert(SUCCEEDED(hResult));
}
