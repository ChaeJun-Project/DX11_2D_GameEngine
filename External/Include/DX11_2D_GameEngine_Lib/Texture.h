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

	UINT m_texture_bind_stage; //�ؽ�ó ���ε� �ܰ�
	UINT m_texture_bind_slot; //�ؽ�ó ���ε� ���� ��ȣ
	Vector2 m_texture_size; //x: �ؽ�ó �ʺ�, y: �ؽ�ó ����
};

