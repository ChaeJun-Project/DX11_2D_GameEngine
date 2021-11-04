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

	void LoadFromFile(const std::string& texture_path) override;
	void SaveFile(const std::string& texture_path) override;

	void Create(const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage);
	void Create(const ComPtr<ID3D11Texture2D>& texture2D);
	
	void BindPipeline() override;
	void BindPipelineRW(const UINT& unordered_bind_slot);

	void Clear();

private:
	const HRESULT& GetScratchImage(const std::wstring& texture_path, ScratchImage& image);

public:
	void SetPipelineStage(const PipelineStage& texture_bind_stage) { m_texture_bind_stage = static_cast<UINT>(texture_bind_stage); }
	void SetBindSlot(const UINT& texture_bind_slot) { m_texture_bind_slot = texture_bind_slot; }

	ID3D11Texture2D* GetTexture2D() const { SAFE_GET_POINTER(m_p_texture2D.Get()); }

	ID3D11RenderTargetView* GetRenderTargetView() const { SAFE_GET_POINTER(m_p_render_target_view.Get()); }
	ID3D11DepthStencilView* GetDepthStencilView() const { SAFE_GET_POINTER(m_p_depth_stencil_view.Get()); }
	ID3D11ShaderResourceView* GetShaderResourceView() const { SAFE_GET_POINTER(m_p_shader_resource_view.Get()); }
	ID3D11UnorderedAccessView* GetUnorderedAccessView() const { SAFE_GET_POINTER(m_p_unordered_access_view.Get()); }
	
	//Texture Bind Stage
	const UINT& GetTextureBindStage() const { return static_cast<UINT>(m_texture_bind_stage); }
	//Texture Bind Slot 
	const UINT& GetTextureBindSlot() const { return static_cast<UINT>(m_texture_bind_slot); }
	//Texture Size
	const UINT& GetWidth() const { return m_texture_desc.Width; }
	const UINT& GetHeight() const { return m_texture_desc.Height; }

private:
	ComPtr<ID3D11Texture2D> m_p_texture2D = nullptr;
	D3D11_TEXTURE2D_DESC m_texture_desc;

	ComPtr<ID3D11RenderTargetView> m_p_render_target_view = nullptr;
	ComPtr<ID3D11DepthStencilView> m_p_depth_stencil_view = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_p_shader_resource_view = nullptr;
	ComPtr<ID3D11UnorderedAccessView> m_p_unordered_access_view = nullptr;

	UINT m_texture_bind_stage = 0; //텍스처 바인드 단계
	UINT m_texture_bind_slot = 0; //t register 바인드 슬롯 번호
	UINT m_unordered_bind_slot = 0; //u register 바인드 슬롯 번호
};

