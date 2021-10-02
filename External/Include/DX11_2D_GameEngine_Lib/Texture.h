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
	Texture(const std::string resource_name);
	~Texture() = default;

	const bool& LoadFromFile(const std::string& texture_path) override;
	void SaveFile(const std::string& texture_path) override;

	void BindPipeline() override;

private:
	const HRESULT& GetScratchImage(const std::wstring& texture_path, ScratchImage& image);

public:
	void SetPipelineStage(const UINT& texture_bind_stage, const UINT& texture_bind_slot)
	{
		this->m_texture_bind_stage = texture_bind_stage;
		this->m_texture_bind_slot = texture_bind_slot;
	}

	ID3D11Texture2D* GetTexture2D() const { SAFE_GET_POINTER(this->m_p_texture.Get()); }
	ID3D11ShaderResourceView* GetShaderResourceView() const { SAFE_GET_POINTER(this->m_p_shader_resource_view.Get()); }
	const UINT& GetTextureBindStage() const { return this->m_texture_bind_stage; }
	const UINT& GetTextureBindSlot() const { return this->m_texture_bind_slot; }
	const Vector2& GetTextureSize() const { return this->m_texture_size; }

private:
	ComPtr<ID3D11Texture2D> m_p_texture = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_p_shader_resource_view = nullptr;

	UINT m_texture_bind_stage; //텍스처 바인드 단계
	UINT m_texture_bind_slot; //텍스처 바인드 슬롯 번호
	Vector2 m_texture_size; //x: 텍스처 너비, y: 텍스처 높이
};

