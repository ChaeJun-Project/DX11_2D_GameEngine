#pragma once
#include "IShader.h"

class GeometryShader : public IShader
{
public:
    GeometryShader();
    ~GeometryShader() = default;

    void Create(
        const std::string & path,			//Shader 파일 경로
        const std::string & function_name,	//Shader 파일에서 사용되는 함수이름
        const std::string & shader_version  //사용할 Shader 버전
    ) override;

public:
    ID3D11GeometryShader* GetGeometryShader() const { SAFE_GET_POINTER(this->m_p_geometry_shader.Get()); }
    ID3DBlob* GetBlob() const { SAFE_GET_POINTER(this->m_p_blob.Get()); }

private:
    //Geometry Shader 관련
    ComPtr<ID3D11GeometryShader> m_p_geometry_shader = nullptr;
    ComPtr<ID3DBlob> m_p_blob = nullptr;
};

