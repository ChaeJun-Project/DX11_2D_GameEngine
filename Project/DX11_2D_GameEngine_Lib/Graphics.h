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
	//ID가 붙으면 com 인터페이스임
	//DX에서 자원을 만듦(여기서 자원은 GPU의 자원임)
	ID3D11Device* m_p_device = nullptr;
	//Device에서 만든 자원을 GPU에 어떻게 사용할지 전달하는 역할
	//명령을 전달
	ID3D11DeviceContext* m_p_device_context = nullptr;

	//DX가 가지고 있는 기본 하부구조
	//IDXGISwapChain의 인터페이스는 화면을 출력하기 전
	//렌더링 된 데이터를 저장함(더블 버퍼링의 백버퍼 역할)
	IDXGISwapChain* m_p_swap_chain = nullptr;

	ID3D11Texture2D* m_p_RT_Tex;
	ID3D11Texture2D* m_p_DS_Tex;

	//출력을 하는 도화지 역할
	ID3D11RenderTargetView* m_p_render_target_view = nullptr;
	
	ID3D11DepthStencilView* m_p_m_pDSV = nullptr;

	//보여지는 영역
	D3D11_VIEWPORT m_viewport;

	//클리어 컬러
	float m_clear_color[4];

	uint m_gpu_memory_size = 0;
	uint m_numerator = 0;
	uint m_denominator = 0;
	std::wstring m_gpu_description = L"";
};