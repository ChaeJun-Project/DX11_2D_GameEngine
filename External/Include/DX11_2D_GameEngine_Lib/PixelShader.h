#pragma once

#include "IShader.h"

class PixelShader final : public IShader
{
public:
    PixelShader() = default;
    ~PixelShader() = default;

    void Create(
        const std::string & path,			//Shader ���� ���
        const std::string & function_name,	//Shader ���Ͽ��� ���Ǵ� �Լ��̸�
        const std::string & shader_version   //����� Shader ����
    ) override;

public:
    ID3D11PixelShader* GetPixelShader() const { SAFE_GET_POINT(this->m_p_pixel_shader.Get()); }
    ID3DBlob* GetBlob() const { SAFE_GET_POINT(this->m_p_blob.Get()); }

private:
    ComPtr<ID3D11PixelShader> m_p_pixel_shader = nullptr;
    ComPtr<ID3DBlob> m_p_blob = nullptr;
};

