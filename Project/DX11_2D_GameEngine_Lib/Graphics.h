#pragma once

class Graphics final
{
public:
	Graphics();
	~Graphics();

	const bool Initialize();

	auto GetDevice() const -> ID3D11Device* { return this->m_p_device; }
	auto GetDeviceContext() const -> ID3D11DeviceContext* { return this->m_p_device_context; }

	//void Resize(const uint& width, const uint& height);
	//void SetViewport(const uint& width, const uint& height);

	void BeginScene();
	void EndScene();

private:
	void CreateSwapChain();
	void CreateRenderTargetView();
	void DeleteRenderTarget();

private:
	//ID�� ������ com �������̽���
	//DX���� �ڿ��� ����(���⼭ �ڿ��� GPU�� �ڿ���)
	ID3D11Device* m_p_device = nullptr;
	//Device���� ���� �ڿ��� GPU�� ��� ������� �����ϴ� ����
	//����� ����
	ID3D11DeviceContext* m_p_device_context = nullptr;

	//DX�� ������ �ִ� �⺻ �Ϻα���
	//IDXGISwapChain�� �������̽��� ȭ���� ����ϱ� ��
	//������ �� �����͸� ������(���� ���۸��� ����� ����)
	IDXGISwapChain* m_p_swap_chain = nullptr;

	ID3D11Texture2D* m_p_RT_Tex;
	ID3D11Texture2D* m_p_DS_Tex;

	//����� �ϴ� ��ȭ�� ����
	ID3D11RenderTargetView* m_p_render_target_view = nullptr;
	
	ID3D11DepthStencilView* m_p_m_pDSV = nullptr;

	//�������� ����
	D3D11_VIEWPORT m_viewport;

	//Ŭ���� �÷�
	float m_clear_color[4];

	uint m_gpu_memory_size = 0;
	uint m_numerator = 0;
	uint m_denominator = 0;
	std::wstring m_gpu_description = L"";
};