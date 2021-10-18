#pragma once

class ConstantBuffer;
class SamplerState;
class BlendState;

class GraphicsManager final : public Singleton<GraphicsManager>
{
	SINGLETON(GraphicsManager);
	GraphicsManager();
	~GraphicsManager();

public:
	const bool Initialize();
	//TODO ��ġ �ٲٱ�(Constant, Sampler, Blend)
	void CreateConstantBuffers();
	void CreateSampler();
	void CreateBlender();

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
	ID3D11Device* GetDevice() const { SAFE_GET_POINTER(this->m_p_device.Get()); }
	ID3D11DeviceContext* GetDeviceContext() const { SAFE_GET_POINTER(this->m_p_device_context.Get()); }

	const std::shared_ptr<ConstantBuffer>& GetConstantBuffer(const CBuffer_BindSlot& bind_slot);

private:
	const bool CreateDeviceAndDeviceContext();
	const bool CreateSwapChain();
	const bool CreateRenderTargetView();
	const bool CreateDepthStencilView();

private:
	//ID�� ������ com �������̽���
	//DX���� �ڿ��� ����(���⼭ �ڿ��� GPU�� �ڿ���, �޸� �Ҵ�)
	ComPtr<ID3D11Device> m_p_device = nullptr;
	//Device���� ���� �ڿ��� GPU�� ��� ������� �����ϴ� ����
	//����� ����
	ComPtr<ID3D11DeviceContext> m_p_device_context = nullptr;

	//DX�� ������ �ִ� �⺻ �Ϻα���
	//IDXGISwapChain�� �������̽��� ȭ���� ����ϱ� ��
	//�������� �����͸� ������(���� ���۸��� ����� ����)
	ComPtr<IDXGISwapChain> m_p_swap_chain = nullptr;

	//����� �ϴ� ��ȭ�� ����
	//Swap Chain�� �� ���۸� ����Ÿ�� ��� ����
	ComPtr<ID3D11RenderTargetView> m_p_render_target_view = nullptr;

	//���� ���� ������ ����
	ComPtr<ID3D11DepthStencilView> m_p_depth_stencil_view = nullptr;

	//�������� ����
	//�������� ���� Ÿ���� �׸��� ������� �ű� �� � ������� �������� ����
	D3D11_VIEWPORT m_viewport;

	//ConstantBuffer�� ������ map
	std::map<CBuffer_BindSlot, std::shared_ptr<ConstantBuffer>> m_p_constant_buffer_map;

	//Sampler�� ������ map
	std::map<std::string, std::shared_ptr<SamplerState>> m_p_sampler_map;

	//Blender�� ������ map
	std::map<std::string, std::shared_ptr<BlendState>> m_p_blender_map;

	//Ŭ���� �÷�
	float m_clear_color[4]; //Init TODO

	//GPU ���� ����
	UINT m_gpu_memory_size = 0;
	UINT m_numerator = 0;
	UINT m_denominator = 0;
	std::wstring m_gpu_description = L"";
};

#define DEVICE GraphicsManager::GetInstance()->GetDevice();
#define DEVICE_CONTEXT GraphicsManager::GetInstance()->GetDeviceConte();