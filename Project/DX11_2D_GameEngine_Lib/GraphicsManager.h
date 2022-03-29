#pragma once

class ConstantBuffer;
class RasterizerState;
class SamplerState;
class BlendState;
class DepthStencilState;

class Texture;

class GraphicsManager final : public Singleton<GraphicsManager>
{
	SINGLETON(GraphicsManager);
	GraphicsManager();
	~GraphicsManager();

public:
	//DirectX11 �ڿ� �ʱ�ȭ
	void Initialize();
	//����ۿ� �׷��� ���� �ʱ�ȭ
	void ClearRenderTarget();
	//����ۿ� �׷��� ����Ÿ�� ����
	void SetRenderTarget();
	//����ۿ� �׷��� ������ ���� �����ְ� �ִ� ���� ���ۿ� ��ü
	void Present();

	//���� ���α׷� ���ο��� �������� �ػ󵵸� ������ ��
	void ResizeWindowByProgram(const UINT& width, const UINT& height);
	//������ ���ؼ� �������� �ػ󵵰� ����� ��
	void ResizeWindowByUser(const UINT& width, const UINT& height);
	//��üȭ������ ����
	void SetFullScreen(const bool& is_full_screen);

public:
	//Get Method
	//const ��� �Լ��� �����Ͽ� �Լ� ������ ��� ������ ������ ����
	ID3D11Device* GetDevice() { SAFE_GET_POINTER(m_p_device.Get()); }
	ID3D11DeviceContext* GetDeviceContext() { SAFE_GET_POINTER(m_p_device_context.Get()); }

	const std::shared_ptr<ConstantBuffer> GetConstantBuffer(const CBuffer_BindSlot& bind_slot);
	const std::shared_ptr<RasterizerState> GetRasterizer(const RasterizerType& rasterizer_type);
	const std::shared_ptr<DepthStencilState> GetDepthStencilState(const DepthStencilType& depth_stencil_type);
	const std::shared_ptr<BlendState> GetBlender(const BlendType& blend_type);

	const std::shared_ptr<Texture>& GetRenderTexture() { return m_p_render_target_view; }
	const std::shared_ptr<Texture>& GetDepthStencilTexture() { return m_p_depth_stencil_view; }
	
private:
	void CreateDeviceAndDeviceContext();
	void CreateSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void CreateConstantBuffers();
	void CreateRasterizer();
	void CreateSampler();
	void CreateDepthStencil();
	void CreateBlender();

	void SetViewport(const UINT& width, const UINT& height);

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
	std::shared_ptr<Texture> m_p_render_target_view;

	//���� ���� ������ ����
	std::shared_ptr<Texture> m_p_depth_stencil_view;

	//�������� ����
	//�������� ���� Ÿ���� �׸��� ������� �ű� �� � ������� �������� ����
	D3D11_VIEWPORT m_viewport;

	//ConstantBuffer�� ������ map
	std::map<CBuffer_BindSlot, std::shared_ptr<ConstantBuffer>> m_p_constant_buffer_map;

	//Rasterizer�� ������ map
	std::map<RasterizerType, std::shared_ptr<RasterizerState>> m_p_rasterizer_map;

	//Sampler�� ������ map
	std::map<std::string, std::shared_ptr<SamplerState>> m_p_sampler_map;

	//DepthStencil�� ������ map
	std::map<DepthStencilType, std::shared_ptr<DepthStencilState>> m_p_depth_stecil_map;

	//Blender�� ������ map
	std::map<BlendType, std::shared_ptr<BlendState>> m_p_blender_map;

	//GPU ���� ����
	UINT m_gpu_memory_size = 0;
	UINT m_numerator = 0;
	UINT m_denominator = 0;
	std::wstring m_gpu_description = L"";
};