#pragma once
#include "IShader.h"

class Texture;

class ComputeShader : public IShader
{
public:
    ComputeShader();
    ~ComputeShader() = default;

    void Create(
        const std::string & path,			//Shader 파일 경로
        const std::string & function_name,	//Shader 파일에서 사용되는 함수이름
        const std::string & shader_version   //사용할 Shader 버전
    ) override;

    void Dispatch(const UINT& thread_group_x_count, const UINT& thread_group_y_count, const UINT& thread_group_z_count);

public:
    ID3D11ComputeShader* GetComputeShader() const { SAFE_GET_POINTER(this->m_p_compute_shader.Get()); }
    ID3DBlob* GetBlob() const { SAFE_GET_POINTER(this->m_p_blob.Get()); }

    void SetGroupThreadCount(const UINT& thread_group_x_count, const UINT& thread_group_y_count, const UINT& thread_group_z_count)
    {
        m_thread_group_x = thread_group_x_count;
        m_thread_group_y = thread_group_y_count;
        m_thread_group_z = thread_group_z_count;
    }

    UINT GetGroupThreadXCount() const { return m_thread_group_x ;}
    UINT GetGroupThreadYCount() const { return m_thread_group_y; }
    UINT GetGroupThreadZCount() const { return m_thread_group_z; }

private:
    ComPtr<ID3D11ComputeShader> m_p_compute_shader = nullptr;
    ComPtr<ID3DBlob> m_p_blob = nullptr;

    //Thread Group
    UINT m_thread_group_x = 0;
    UINT m_thread_group_y = 0;
    UINT m_thread_group_z = 0;
};

