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
	//DirectX11 자원 초기화
	void Initialize();
	//백버퍼에 그려진 내용 초기화
	void ClearRenderTarget();
	//백버퍼에 그려질 렌더타겟 설정
	void SetRenderTarget();
	//백버퍼에 그려진 내용을 현재 보여주고 있는 전면 버퍼와 교체
	void Present();

	//응용 프로그램 내부에서 윈도우의 해상도를 변경할 때
	void ResizeWindowByProgram(const UINT& width, const UINT& height);
	//유저에 의해서 윈도우의 해상도가 변경될 때
	void ResizeWindowByUser(const UINT& width, const UINT& height);
	//전체화면으로 변경
	void SetFullScreen(const bool& is_full_screen);

public:
	//Get Method
	//const 멤버 함수로 구현하여 함수 내에서 멤버 변수의 수정을 방지
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
	//ID가 붙으면 com 인터페이스임
	//DX에서 자원을 만듦(여기서 자원은 GPU의 자원임, 메모리 할당)
	ComPtr<ID3D11Device> m_p_device = nullptr;
	//Device에서 만든 자원을 GPU에 어떻게 사용할지 전달하는 역할
	//명령을 전달
	ComPtr<ID3D11DeviceContext> m_p_device_context = nullptr;

	//DX가 가지고 있는 기본 하부구조
	//IDXGISwapChain의 인터페이스는 화면을 출력하기 전
	//렌더링된 데이터를 저장함(더블 버퍼링의 백버퍼 역할)
	ComPtr<IDXGISwapChain> m_p_swap_chain = nullptr;

	//출력을 하는 도화지 역할
	//Swap Chain의 백 버퍼를 렌더타겟 뷰로 만듦
	std::shared_ptr<Texture> m_p_render_target_view;

	//깊이 값을 가지고 있음
	std::shared_ptr<Texture> m_p_depth_stencil_view;

	//보여지는 영역
	//렌더링된 렌더 타켓의 그림을 윈도우로 옮길 때 어떤 방식으로 가져올지 설정
	D3D11_VIEWPORT m_viewport;

	//ConstantBuffer를 저장할 map
	std::map<CBuffer_BindSlot, std::shared_ptr<ConstantBuffer>> m_p_constant_buffer_map;

	//Rasterizer를 저장할 map
	std::map<RasterizerType, std::shared_ptr<RasterizerState>> m_p_rasterizer_map;

	//Sampler를 저장할 map
	std::map<std::string, std::shared_ptr<SamplerState>> m_p_sampler_map;

	//DepthStencil을 저장할 map
	std::map<DepthStencilType, std::shared_ptr<DepthStencilState>> m_p_depth_stecil_map;

	//Blender를 저장할 map
	std::map<BlendType, std::shared_ptr<BlendState>> m_p_blender_map;

	//GPU 관련 정보
	UINT m_gpu_memory_size = 0;
	UINT m_numerator = 0;
	UINT m_denominator = 0;
	std::wstring m_gpu_description = L"";
};