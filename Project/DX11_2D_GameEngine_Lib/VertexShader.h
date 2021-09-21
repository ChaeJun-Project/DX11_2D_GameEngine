#pragma once

#include "IShader.h"

class InputLayout;

//VertexShader는 InputLayout 정보를 가지고 있음
class VertexShader final : public IShader
{
public:
    VertexShader() = default;
    ~VertexShader() = default;

    void Create(
        const std::string& path,			//Shader 파일 경로
        const std::string& function_name,	//Shader 파일에서 사용되는 함수이름
        const std::string& shader_version   //사용할 Shader 버전
    ) override;

public:
    ID3D11VertexShader* GetVertexShader() const { SAFE_GET_POINT(this->m_p_vertex_shader.Get()); }
    ID3DBlob* GetBlob() const { SAFE_GET_POINT(this->m_p_blob.Get()); }

private:
    //Vertex Shader 관련
    ComPtr<ID3D11VertexShader> m_p_vertex_shader = nullptr;
    ComPtr<ID3DBlob> m_p_blob = nullptr;

    //Input Layout 관련
    ComPtr<InputLayout> m_p_input_layout_class = nullptr;
};

