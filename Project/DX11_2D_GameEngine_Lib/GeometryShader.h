#pragma once
#include "IShader.h"

class GeometryShader : public IShader
{
public:
    GeometryShader();
    ~GeometryShader() = default;

    void Create(
        const std::string & path,			//Shader ���� ���
        const std::string & function_name,	//Shader ���Ͽ��� ���Ǵ� �Լ��̸�
        const std::string & shader_version  //����� Shader ����
    ) override;

public:
    ID3D11GeometryShader* GetGeometryShader() const { SAFE_GET_POINTER(this->m_p_geometry_shader.Get()); }
    ID3DBlob* GetBlob() const { SAFE_GET_POINTER(this->m_p_blob.Get()); }

private:
    //Geometry Shader ����
    ComPtr<ID3D11GeometryShader> m_p_geometry_shader = nullptr;
    ComPtr<ID3DBlob> m_p_blob = nullptr;
};

