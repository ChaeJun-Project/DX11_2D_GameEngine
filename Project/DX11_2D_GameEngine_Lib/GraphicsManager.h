#pragma once

class ConstantBuffer;

class GraphicsManager final : public Singleton<GraphicsManager>
{
    SINGLETON(GraphicsManager);
	GraphicsManager();
	~GraphicsManager();
	
public:
	//���� ���α׷� ���ο��� �������� �ػ󵵸� ������ ��
	void ResizeWindowByProgram(const UINT& width, const UINT& height);
	//������ ���ؼ� �������� �ػ󵵰� ����� ��
	void ResizeWindowByUser(const UINT& width, const UINT& height);
	void SetFullScreen(const bool& is_full_screen);
	void SetViewport(const UINT& width, const UINT& height);
	
	void BeginScene();
	void EndScene();

public:
	//Get Method
	//const ��� �Լ��� �����Ͽ� �Լ� ������ ��� ������ ������ ����
	ID3D11Device* GetDevice() const { SAFE_GET_POINTER(this->m_p_device);}
	ID3D11DeviceContext* GetDeviceContext() const { SAFE_GET_POINTER(this->m_p_device_context); }

	std::shared_ptr<ConstantBuffer> GetConstantBuffer(const CBuffer_BindSlot& bind_slot);

private:
	const bool Initialize();
    const bool CreateDeviceAndDeviceContext();
	const bool CreateSwapChain();
	const bool CreateRenderTargetView();
	const bool CreateDepthStencilView();

	void CreateConstantBuffers();

private:
	//ID�� ������ com �������̽���
	//DX���� �ڿ��� ����(���⼭ �ڿ��� GPU�� �ڿ���, �޸� �Ҵ�)
	ID3D11Device* m_p_device = nullptr;
	//Device���� ���� �ڿ��� GPU�� ��� ������� �����ϴ� ����
	//����� ����
	ID3D11DeviceContext* m_p_device_context = nullptr;

	//DX�� ������ �ִ� �⺻ �Ϻα���
	//IDXGISwapChain�� �������̽��� ȭ���� ����ϱ� ��
	//�������� �����͸� ������(���� ���۸��� ����� ����)
	IDXGISwapChain* m_p_swap_chain = nullptr;

	//����� �ϴ� ��ȭ�� ����
	//Swap Chain�� �� ���۸� ����Ÿ�� ��� ����
	ID3D11RenderTargetView* m_p_render_target_view = nullptr;

	//���� ���� ������ ����
	ID3D11DepthStencilView* m_p_depth_stencil_view = nullptr;
	ID3D11Texture2D* m_p_depth_stencil_texture = nullptr;

	//�������� ����
	//�������� ���� Ÿ���� �׸��� ������� �ű� �� � ������� �������� ����
	D3D11_VIEWPORT m_viewport;

	//ConstantBuffer�� ������ map
	std::map<CBuffer_BindSlot, std::shared_ptr<ConstantBuffer>> m_p_constant_buffer_map;

	//Ŭ���� �÷�
	float m_clear_color[4]; //Init TODO

	//GPU ���� ����
	UINT m_gpu_memory_size = 0;
	UINT m_numerator = 0;
	UINT m_denominator = 0;
	std::wstring m_gpu_description = L"";
};