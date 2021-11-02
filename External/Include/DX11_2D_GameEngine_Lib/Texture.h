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
	Texture(const std::string resource_name);
	~Texture() = default;

	void LoadFromFile(const std::string& texture_path) override;
	void SaveFile(const std::string& texture_path) override;
	
	void BindPipeline() override;

public:
    void Create(const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage);
	void Create(const ComPtr<ID3D11Texture2D>& texture2D);

private:
	const HRESULT& GetScratchImage(const std::wstring& texture_path, ScratchImage& image);

public:
	void SetPipelineStage(const UINT& texture_bind_stage, const UINT& texture_bind_slot)
	{
		this->m_texture_bind_stage = texture_bind_stage;
		this->m_texture_bind_slot = texture_bind_slot;
	}

	ID3D11Texture2D* GetTexture2D() const { SAFE_GET_POINTER(this->m_p_texture2D.Get()); }

	ID3D11RenderTargetView* GetRenderTargetView() const { SAFE_GET_POINTER(this->m_p_render_target_view.Get()); }
	ID3D11DepthStencilView* GetDepthStencilView() const { SAFE_GET_POINTER(this->m_p_depth_stencil_view.Get()); }
	ID3D11ShaderResourceView* GetShaderResourceView() const { SAFE_GET_POINTER(this->m_p_shader_resource_view.Get()); }
	ID3D11UnorderedAccessView* GetUnorderedAccessView() const { SAFE_GET_POINTER(this->m_p_unordered_access_view.Get()); }
	
	//Texture Bind Stage
	const UINT& GetTextureBindStage() const { return static_cast<UINT>(this->m_texture_bind_stage); }
	//Texture Bind Slot 
	const UINT& GetTextureBindSlot() const { return static_cast<UINT>(this->m_texture_bind_slot); }
	//Texture Size
	const UINT& GetWidth() const { return this->m_texture_desc.Width; }
	const UINT& GetHeight() const { return this->m_texture_desc.Height; }

private:
	ComPtr<ID3D11Texture2D> m_p_texture2D = nullptr;
	D3D11_TEXTURE2D_DESC m_texture_desc;

	ComPtr<ID3D11RenderTargetView> m_p_render_target_view = nullptr;
	ComPtr<ID3D11DepthStencilView> m_p_depth_stencil_view = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_p_shader_resource_view = nullptr;
	ComPtr<ID3D11UnorderedAccessView> m_p_unordered_access_view = nullptr;

	int m_texture_bind_stage = -1; //�ؽ�ó ���ε� �ܰ�
	int m_texture_bind_slot = -1; //�ؽ�ó ���ε� ���� ��ȣ
};

