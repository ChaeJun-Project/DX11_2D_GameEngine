#pragma once
#include "IResource.h"

//�ؽ�ó�� �ε��ϱ� ���� DirectXTex ���̺귯�� ��� ����
#include <DirectXTex/DirectXTex.h>

//���� ���̺귯�� ����
#ifdef _DEBUG //����� ����� ��
#pragma comment(lib, "DirectXTex/Debug/DirectXTex_Debug.lib")
#else //������ ����� ��
#pragma comment(lib, "DirectXTex/Release/DirectXTex_Release.lib")
#endif


class Texture : public IResource
{
public:
	Texture() = default;
	~Texture() = default;

	const bool& LoadTextureFromFile(const std::string& texture_path);

private:
    const HRESULT& GetScratchImage(const std::wstring& texture_path, ScratchImage& image);

public:
	ID3D11ShaderResourceView* GetShaderResourceView() const { SAFE_GET_POINTER(this->m_p_shader_resource_view.Get()); }

private:
    ComPtr<ID3D11Texture2D> m_p_texture;
	ComPtr<ID3D11ShaderResourceView> m_p_shader_resource_view;
};

