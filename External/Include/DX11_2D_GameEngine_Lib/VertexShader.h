#pragma once

#include "IShader.h"

class VertexShader final : public IShader
{
public:
    VertexShader() = default;
    ~VertexShader() = default;

    void Create(
        const std::string& path,			//Shader ���� ���
        const std::string& function_name,	//Shader ���Ͽ��� ���Ǵ� �Լ��̸�
        const std::string& shader_version   //����� Shader ����
    ) override;

public:
    ID3D11VertexShader* GetVertexShader() const { SAFE_GET_POINT(this->m_p_vertex_shader.Get()); }
    ID3DBlob* GetBlob() const { SAFE_GET_POINT(this->m_p_blob.Get()); }

private:
    ComPtr<ID3D11VertexShader> m_p_vertex_shader = nullptr;
    ComPtr<ID3DBlob> m_p_blob = nullptr;
};

