#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics()
{
	//뷰 포트 구조체 0으로 초기화
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
}

Graphics::~Graphics()
{
	//DirectX 자원 메모리 해제
	//Reference Count - 1

	SAFE_RELEASE(m_p_render_target_view);
	SAFE_RELEASE(m_p_render_target_texture);

	SAFE_RELEASE(m_p_depth_stencil_view);
	SAFE_RELEASE(m_p_depth_stencil_texture);

	//전체모드인 경우
	if (m_p_swap_chain && Settings::GetInstance()->IsFullScreen())
	{
		//스왑체인 메모리 해제 전에 윈도우 모드로 변경해야 예외가 발생하지 않음
		m_p_swap_chain->SetFullscreenState(false, nullptr);
	}
	SAFE_RELEASE(m_p_swap_chain);

	SAFE_RELEASE(m_p_device_context);
	SAFE_RELEASE(m_p_device);
}

const bool Graphics::Initialize()
{
	//지원하는 해상도 찾기(참고: https://copynull.tistory.com/240)
	//========================================================================================================
	IDXGIFactory* p_DXGI_factory = nullptr;
	auto hResult = CreateDXGIFactory
	(
		__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(&p_DXGI_factory)
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	IDXGIAdapter* p_DXGI_adapter = nullptr;
	hResult = p_DXGI_factory->EnumAdapters(0, &p_DXGI_adapter);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	IDXGIOutput* p_DXGI_adapter_output = nullptr;
	hResult = p_DXGI_adapter->EnumOutputs(0, &p_DXGI_adapter_output);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	UINT display_mode_count = 0;
	hResult = p_DXGI_adapter_output->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&display_mode_count,
		nullptr
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	auto display_mode_list = new DXGI_MODE_DESC[display_mode_count];
	hResult = p_DXGI_adapter_output->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&display_mode_count,
		display_mode_list
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	for (UINT i = 0; i < display_mode_count; i++)
	{
		bool is_check = true;
		is_check &= display_mode_list[i].Width == static_cast<UINT>(Settings::GetInstance()->GetWindowWidth());
		is_check &= display_mode_list[i].Height == static_cast<UINT>(Settings::GetInstance()->GetWindowHeight());

		if (is_check)
		{
			m_numerator = display_mode_list[i].RefreshRate.Numerator;
			m_denominator = display_mode_list[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC adapter_desc;
	hResult = p_DXGI_adapter->GetDesc(&adapter_desc);

	m_gpu_memory_size = static_cast<UINT>(adapter_desc.DedicatedVideoMemory / 1024 / 1024);
	m_gpu_description = adapter_desc.Description;

	//콘솔창에 GPU정보 출력
	std::wcout << "GPU Name: " << m_gpu_description << std::endl;
	std::cout << "GPU Memory Size: " << m_gpu_memory_size << std::endl;
	std::cout << "Screen Rate: " << m_numerator/ m_denominator << std::endl;
	//========================================================================================================

	//Device와 DeviceContext 생성
	auto result = CreateDeviceAndDeviceContext();
	assert(result);
	if (!result)
		return false;

	//Swap Chain 생성
	result = CreateSwapChain();
	assert(result);
	if (!result)
		return false;

	auto settings = Settings::GetInstance();
	ResizeWindowByUser(settings->GetWindowWidth(), settings->GetWindowHeight());

	return true;
}

void Graphics::ResizeWindowByProgram(const UINT& width, const UINT& height)
{
	if (m_p_swap_chain)
	{
		DXGI_MODE_DESC desc;

		//해상도 설정
		desc.Width = width;
		desc.Height = height;

		//주사율 설정
		desc.RefreshRate.Numerator = Settings::GetInstance()->IsVsync() ? m_numerator : 0;
		desc.RefreshRate.Denominator = Settings::GetInstance()->IsVsync() ? m_denominator : 0;

		//픽셀 포멧 설정
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		//모니터 출력 방식 설정
		desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;

		auto hResult = m_p_swap_chain->ResizeTarget(&desc);
		assert(SUCCEEDED(hResult));
	}
}

void Graphics::ResizeWindowByUser(const UINT& width, const UINT& height)
{
	//RenderTargetView 자원 해제
	SAFE_RELEASE(m_p_render_target_texture);

	if (m_p_swap_chain)
	{
		auto hResult = m_p_swap_chain->ResizeBuffers
		(
			1,
			0,
			0,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			0
		);
		assert(SUCCEEDED(hResult));
	}

	//RenderTarget관련 자원 생성
	CreateRenderTargetView();

	//Viewport 재설정
	SetViewport(width, height);
}

void Graphics::SetFullScreen(const bool& is_full_screen)
{
	if (m_p_swap_chain)
		m_p_swap_chain->SetFullscreenState(is_full_screen, nullptr);
}

void Graphics::SetViewport(const float& width, const float& height)
{
	//뷰포트의 시작점 설정
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	//뷰포트의 크기 설정
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);

	//뷰포트의 최대, 최소 깊이값 설정
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
}

void Graphics::BeginScene()
{
	if (m_p_device_context && m_p_render_target_view)
	{
		//백 버퍼에 그려진 내용(render_target_view)을 Output_Merger의 렌더타겟으로 설정
		m_p_device_context->OMSetRenderTargets(1, &m_p_render_target_view, nullptr);
		//설정한 뷰포트 등록
		m_p_device_context->RSSetViewports(1, &m_viewport);
		//백 버퍼(render_target_view)에 그려진 내용 지우기
		m_p_device_context->ClearRenderTargetView(m_p_render_target_view, m_clear_color);
	}
}

void Graphics::EndScene()
{
	if (m_p_swap_chain)
	{
		//백 버퍼에 그린 내용을 전면 버퍼로 스왑
		auto hResult = m_p_swap_chain->Present(Settings::GetInstance()->IsVsync() ? 1 : 0, 0);
		assert(SUCCEEDED(hResult));
	}
}

const bool Graphics::CreateSwapChain()
{
	auto settings = Settings::GetInstance();

	//Swap Chain 구조체 설정
	//========================================================================================================
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	//만들어질 백 버퍼와 전면 버퍼의 해상도 설정
	//Swap Chain 생성 직후 윈도우의 크기만큼 재설정하기 때문에 지금은 0으로 초기화
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;

	//주사율(프레임) 설정
	//분자
	desc.BufferDesc.RefreshRate.Numerator = settings->IsVsync() ? m_numerator : 0;
	//분모
	desc.BufferDesc.RefreshRate.Denominator = settings->IsVsync() ? m_denominator : 1;

	//버퍼의 픽셀 포멧
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//모니터 출력 방식
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferCount = 1; //백버퍼

	//해당 자원의 사용목적
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//샘플 수준
	//Sample:특정 데이터를 추출
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	//렌더링 출력 대상 윈도우 설정
	desc.OutputWindow = settings->GetWindowHandle();

	//창모드 On/Off
	//창모드를 원하면 TRUE, 전체화면을 원하면 FALSE
	desc.Windowed = !settings->IsFullScreen();

	//Swap 할 때 방식 설정
	//DXGI_SWAP_EFFECT_DISCARD: Swap 할 때 완료한 데이터를 저장하지 않고 삭제
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//========================================================================================================

	//스왑체인 생성
	IDXGIDevice* p_DXGI_device = nullptr;
	IDXGIAdapter* p_DXGI_adapter = nullptr;
	IDXGIFactory* p_DXGI_factory = nullptr;

	//해당 타입의 객체의 ID를 바탕으로 실제 객체의 주소를 받아옴
	//Get IDXGIDevice
	auto hResult = m_p_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&p_DXGI_device);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	//Get IDXGIAdapter
	p_DXGI_device->GetParent(__uuidof(IDXGIAdapter), (void**)&p_DXGI_adapter);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	//Get IDXGIFactory
	p_DXGI_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&p_DXGI_factory);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	//Create Swap Chain
	p_DXGI_factory->CreateSwapChain(m_p_device, &desc, &m_p_swap_chain);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	return true;
}

const bool Graphics::CreateDeviceAndDeviceContext()
{
	UINT iFlag = 0;

	//플래그 값 참고: https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag?redirectedfrom=MSDN
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	//DirectX 버전 설정
	std::vector<D3D_FEATURE_LEVEL> feature_levels
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	auto hResult = D3D11CreateDevice
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, //컴퓨터에 내장된 그래픽카드를 그대로 사용한다는 뜻
		nullptr,
		iFlag,
		0,
		0,
		D3D11_SDK_VERSION,
		&m_p_device,
		&feature_levels[1],
		&m_p_device_context
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	//어댑터가 지원하는 품질 수준 제공
	UINT iMultiSampleLevel = 0;

	m_p_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iMultiSampleLevel);
	assert(iMultiSampleLevel > 0);
	if (iMultiSampleLevel < 0)
	{
		return false;
	}

	return true;
}

const bool Graphics::CreateRenderTargetView()
{
	if (m_p_swap_chain)
	{
		auto hResult = m_p_swap_chain->GetBuffer
		(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&m_p_render_target_texture)
		);
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return false;

		if(m_p_device)
		hResult = m_p_device->CreateRenderTargetView
		(
			m_p_render_target_texture,
			nullptr,
			&m_p_render_target_view
		);
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return false;

		SAFE_RELEASE(m_p_render_target_view);

		return true;
	}

	return false;
}

const bool Graphics::CreateDepthStencilView()
{
	return false;
}
