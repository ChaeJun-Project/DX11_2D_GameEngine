#pragma once
#include "IResource.h"

//텍스처를 로드하기 위한 DirectXTex 라이브러리 헤더 포함
#include <DirectXTex/DirectXTex.h>

//정적 라이브러리 연결
#ifdef _DEBUG //디버그 모드일 때
#pragma comment(lib, "DirectXTex/Debug/DirectXTex_Debug.lib")
#else //릴리즈 모드일 때
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

