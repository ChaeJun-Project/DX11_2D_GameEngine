#pragma once
#include "IShader.h"

class Texture;

class ComputeShader : public IShader
{
public:
    ComputeShader(const UINT& thread_group_x, const UINT& thread_group_y, const UINT& thread_group_z);
    ~ComputeShader() = default;

    void Create(
        const std::string & path,			//Shader ���� ���
        const std::string & function_name,	//Shader ���Ͽ��� ���Ǵ� �Լ��̸�
        const std::string & shader_version   //����� Shader ����
    ) override;

 
    void Dispatch(const UINT& thread_group_x, const UINT& thread_group_y, const UINT& thread_group_z);

private:
    void BindPipeline();
    void Clear();

public:
    ID3D11ComputeShader* GetComputeShader() const { SAFE_GET_POINTER(this->m_p_compute_shader.Get()); }
    ID3DBlob* GetBlob() const { SAFE_GET_POINTER(this->m_p_blob.Get()); }

    void SetTargetTexture(const std::shared_ptr<Texture>& p_texture) { m_p_texture = p_texture; }

private:
    ComPtr<ID3D11ComputeShader> m_p_compute_shader = nullptr;
    ComPtr<ID3DBlob> m_p_blob = nullptr;

    std::shared_ptr<Texture> m_p_texture = nullptr;

    //Thread Group
    UINT m_thread_group_x = 0;
    UINT m_thread_group_y = 0;
    UINT m_thread_group_z = 0;
};

