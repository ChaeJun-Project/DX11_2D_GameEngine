#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics()
{
	//�� ��Ʈ ����ü 0���� �ʱ�ȭ
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
}

Graphics::~Graphics()
{
	//DirectX �ڿ� �޸� ����
	//Reference Count - 1

	SAFE_RELEASE(m_p_render_target_view);
	SAFE_RELEASE(m_p_render_target_texture);

	SAFE_RELEASE(m_p_depth_stencil_view);
	SAFE_RELEASE(m_p_depth_stencil_texture);

	//��ü����� ���
	if (m_p_swap_chain && Settings::GetInstance()->IsFullScreen())
	{
		//����ü�� �޸� ���� ���� ������ ���� �����ؾ� ���ܰ� �߻����� ����
		m_p_swap_chain->SetFullscreenState(false, nullptr);
	}
	SAFE_RELEASE(m_p_swap_chain);

	SAFE_RELEASE(m_p_device_context);
	SAFE_RELEASE(m_p_device);
}

const bool Graphics::Initialize()
{
	//�����ϴ� �ػ� ã��(����: https://copynull.tistory.com/240)
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

	//�ܼ�â�� GPU���� ���
	std::wcout << "GPU Name: " << m_gpu_description << std::endl;
	std::cout << "GPU Memory Size: " << m_gpu_memory_size << std::endl;
	std::cout << "Screen Rate: " << m_numerator/ m_denominator << std::endl;
	//========================================================================================================

	//Device�� DeviceContext ����
	auto result = CreateDeviceAndDeviceContext();
	assert(result);
	if (!result)
		return false;

	//Swap Chain ����
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

		//�ػ� ����
		desc.Width = width;
		desc.Height = height;

		//�ֻ��� ����
		desc.RefreshRate.Numerator = Settings::GetInstance()->IsVsync() ? m_numerator : 0;
		desc.RefreshRate.Denominator = Settings::GetInstance()->IsVsync() ? m_denominator : 0;

		//�ȼ� ���� ����
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		//����� ��� ��� ����
		desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;

		auto hResult = m_p_swap_chain->ResizeTarget(&desc);
		assert(SUCCEEDED(hResult));
	}
}

void Graphics::ResizeWindowByUser(const UINT& width, const UINT& height)
{
	//RenderTargetView �ڿ� ����
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

	//RenderTarget���� �ڿ� ����
	CreateRenderTargetView();

	//Viewport �缳��
	SetViewport(width, height);
}

void Graphics::SetFullScreen(const bool& is_full_screen)
{
	if (m_p_swap_chain)
		m_p_swap_chain->SetFullscreenState(is_full_screen, nullptr);
}

void Graphics::SetViewport(const float& width, const float& height)
{
	//����Ʈ�� ������ ����
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	//����Ʈ�� ũ�� ����
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);

	//����Ʈ�� �ִ�, �ּ� ���̰� ����
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
}

void Graphics::BeginScene()
{
	if (m_p_device_context && m_p_render_target_view)
	{
		//�� ���ۿ� �׷��� ����(render_target_view)�� Output_Merger�� ����Ÿ������ ����
		m_p_device_context->OMSetRenderTargets(1, &m_p_render_target_view, nullptr);
		//������ ����Ʈ ���
		m_p_device_context->RSSetViewports(1, &m_viewport);
		//�� ����(render_target_view)�� �׷��� ���� �����
		m_p_device_context->ClearRenderTargetView(m_p_render_target_view, m_clear_color);
	}
}

void Graphics::EndScene()
{
	if (m_p_swap_chain)
	{
		//�� ���ۿ� �׸� ������ ���� ���۷� ����
		auto hResult = m_p_swap_chain->Present(Settings::GetInstance()->IsVsync() ? 1 : 0, 0);
		assert(SUCCEEDED(hResult));
	}
}

const bool Graphics::CreateSwapChain()
{
	auto settings = Settings::GetInstance();

	//Swap Chain ����ü ����
	//========================================================================================================
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	//������� �� ���ۿ� ���� ������ �ػ� ����
	//Swap Chain ���� ���� �������� ũ�⸸ŭ �缳���ϱ� ������ ������ 0���� �ʱ�ȭ
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;

	//�ֻ���(������) ����
	//����
	desc.BufferDesc.RefreshRate.Numerator = settings->IsVsync() ? m_numerator : 0;
	//�и�
	desc.BufferDesc.RefreshRate.Denominator = settings->IsVsync() ? m_denominator : 1;

	//������ �ȼ� ����
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//����� ��� ���
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferCount = 1; //�����

	//�ش� �ڿ��� ������
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//���� ����
	//Sample:Ư�� �����͸� ����
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	//������ ��� ��� ������ ����
	desc.OutputWindow = settings->GetWindowHandle();

	//â��� On/Off
	//â��带 ���ϸ� TRUE, ��üȭ���� ���ϸ� FALSE
	desc.Windowed = !settings->IsFullScreen();

	//Swap �� �� ��� ����
	//DXGI_SWAP_EFFECT_DISCARD: Swap �� �� �Ϸ��� �����͸� �������� �ʰ� ����
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//========================================================================================================

	//����ü�� ����
	IDXGIDevice* p_DXGI_device = nullptr;
	IDXGIAdapter* p_DXGI_adapter = nullptr;
	IDXGIFactory* p_DXGI_factory = nullptr;

	//�ش� Ÿ���� ��ü�� ID�� �������� ���� ��ü�� �ּҸ� �޾ƿ�
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

	//�÷��� �� ����: https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag?redirectedfrom=MSDN
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	//DirectX ���� ����
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
		D3D_DRIVER_TYPE_HARDWARE, //��ǻ�Ϳ� ����� �׷���ī�带 �״�� ����Ѵٴ� ��
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

	//����Ͱ� �����ϴ� ǰ�� ���� ����
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
