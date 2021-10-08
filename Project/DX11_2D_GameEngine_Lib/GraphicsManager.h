#pragma once

class ConstantBuffer;

class GraphicsManager final : public Singleton<GraphicsManager>
{
    SINGLETON(GraphicsManager);
	GraphicsManager();
	~GraphicsManager();
	
public:
	//응용 프로그램 내부에서 윈도우의 해상도를 변경할 때
	void ResizeWindowByProgram(const UINT& width, const UINT& height);
	//유저에 의해서 윈도우의 해상도가 변경될 때
	void ResizeWindowByUser(const UINT& width, const UINT& height);
	void SetFullScreen(const bool& is_full_screen);
	void SetViewport(const UINT& width, const UINT& height);
	
	void BeginScene();
	void EndScene();

public:
	//Get Method
	//const 멤버 함수로 구현하여 함수 내에서 멤버 변수의 수정을 방지
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
	//ID가 붙으면 com 인터페이스임
	//DX에서 자원을 만듦(여기서 자원은 GPU의 자원임, 메모리 할당)
	ID3D11Device* m_p_device = nullptr;
	//Device에서 만든 자원을 GPU에 어떻게 사용할지 전달하는 역할
	//명령을 전달
	ID3D11DeviceContext* m_p_device_context = nullptr;

	//DX가 가지고 있는 기본 하부구조
	//IDXGISwapChain의 인터페이스는 화면을 출력하기 전
	//렌더링된 데이터를 저장함(더블 버퍼링의 백버퍼 역할)
	IDXGISwapChain* m_p_swap_chain = nullptr;

	//출력을 하는 도화지 역할
	//Swap Chain의 백 버퍼를 렌더타겟 뷰로 만듦
	ID3D11RenderTargetView* m_p_render_target_view = nullptr;

	//깊이 값을 가지고 있음
	ID3D11DepthStencilView* m_p_depth_stencil_view = nullptr;
	ID3D11Texture2D* m_p_depth_stencil_texture = nullptr;

	//보여지는 영역
	//렌더링된 렌더 타켓의 그림을 윈도우로 옮길 때 어떤 방식으로 가져올지 설정
	D3D11_VIEWPORT m_viewport;

	//ConstantBuffer를 저장할 map
	std::map<CBuffer_BindSlot, std::shared_ptr<ConstantBuffer>> m_p_constant_buffer_map;

	//클리어 컬러
	float m_clear_color[4]; //Init TODO

	//GPU 관련 정보
	UINT m_gpu_memory_size = 0;
	UINT m_numerator = 0;
	UINT m_denominator = 0;
	std::wstring m_gpu_description = L"";
};