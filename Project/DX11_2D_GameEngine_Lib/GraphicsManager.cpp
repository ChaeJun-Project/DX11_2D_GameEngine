#include "stdafx.h"
#include "GraphicsManager.h"

#include "Core.h"
#include "Settings.h"

#include "ResourceManager.h"
#include "Texture.h"

#include "RenderManager.h"

GraphicsManager::GraphicsManager()
{
	//뷰 포트 구조체 0으로 초기화
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
}

GraphicsManager::~GraphicsManager()
{
	auto p_settings = Core::GetInstance()->GetSettings();
	//전체모드인 경우
	if (m_p_swap_chain && p_settings->IsFullScreen())
	{
		//스왑체인 메모리 해제 전에 윈도우 모드로 변경해야 예외가 발생하지 않음
		m_p_swap_chain->SetFullscreenState(false, nullptr);
	}

	m_p_render_target_view.reset();
	m_p_depth_stencil_view.reset();

	//Constant Buffer Map 초기화
	for (auto& constant_buffer : m_p_constant_buffer_map)
	{
		constant_buffer.second.reset();
	}
	m_p_constant_buffer_map.clear();

	//Sampler Vector 초기화
	for (auto& sampler : m_p_sampler_map)
	{
		sampler.second.reset();
	}
	m_p_sampler_map.clear();

	//Blender Map 초기화
	for (auto& blender : m_p_blender_map)
	{
		blender.second.reset();
	}
	m_p_blender_map.clear();
}

void GraphicsManager::Initialize()
{
	//지원하는 해상도 찾기(참고: https://copynull.tistory.com/240)
	//========================================================================================================
	ComPtr<IDXGIFactory> p_DXGI_factory = nullptr;
	auto hResult = CreateDXGIFactory
	(
		__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(p_DXGI_factory.GetAddressOf())
	);
	assert(SUCCEEDED(hResult));

	ComPtr<IDXGIAdapter> p_DXGI_adapter = nullptr;
	hResult = p_DXGI_factory->EnumAdapters(0, p_DXGI_adapter.GetAddressOf());
	assert(SUCCEEDED(hResult));

	ComPtr<IDXGIOutput> p_DXGI_adapter_output = nullptr;
	hResult = p_DXGI_adapter->EnumOutputs(0, p_DXGI_adapter_output.GetAddressOf());
	assert(SUCCEEDED(hResult));

	UINT display_mode_count = 0;
	hResult = p_DXGI_adapter_output->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&display_mode_count,
		nullptr
	);
	assert(SUCCEEDED(hResult));

	auto display_mode_list = new DXGI_MODE_DESC[display_mode_count];
	hResult = p_DXGI_adapter_output->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&display_mode_count,
		display_mode_list
	);
	assert(SUCCEEDED(hResult));

	auto settings = Core::GetInstance()->GetSettings();
	auto current_window_width = settings->GetWindowWidth();
	auto current_window_height = settings->GetWindowHeight();

	for (UINT i = 0; i < display_mode_count; i++)
	{
		bool is_check = true;
		is_check &= display_mode_list[i].Width == static_cast<UINT>(current_window_width);
		is_check &= display_mode_list[i].Height == static_cast<UINT>(current_window_height);

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
	std::cout << "Screen Rate: " << m_numerator / m_denominator << std::endl;

	SATE_DELETE_ARRAY(display_mode_list);
	//========================================================================================================

	//Device와 DeviceContext 생성
	CreateDeviceAndDeviceContext();
	//Swap Chain 생성
	CreateSwapChain();
	//Constant Buffer 생성
	CreateConstantBuffers();
	//Rasterizer 생성
	CreateRasterizer();
	//Sampler 생성
	CreateSampler();
	//DepthStencil 생성
	CreateDepthStencil();
	//Blender 생성
	CreateBlender();


	ResizeWindowByUser(settings->GetWindowWidth(), settings->GetWindowHeight());
}

void GraphicsManager::ResizeWindowByProgram(const UINT& width, const UINT& height)
{
	if (m_p_swap_chain)
	{
		auto settings = Core::GetInstance()->GetSettings();

		DXGI_MODE_DESC desc;

		//해상도 설정
		desc.Width = width;
		desc.Height = height;

		//주사율 설정
		desc.RefreshRate.Numerator = settings->IsVsync() ? m_numerator : 0;
		desc.RefreshRate.Denominator = settings->IsVsync() ? m_denominator : 0;

		//픽셀 포멧 설정
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		//모니터 출력 방식 설정
		desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;

		auto hResult = m_p_swap_chain->ResizeTarget(&desc);
		assert(SUCCEEDED(hResult));
	}
}

void GraphicsManager::ResizeWindowByUser(const UINT& width, const UINT& height)
{
	if (m_p_render_target_view && m_p_depth_stencil_view)
	{
		m_p_render_target_view->ReleaseTexture();
		m_p_render_target_view->ReleaseRenderTargetView();

		m_p_depth_stencil_view->ReleaseTexture();
		m_p_depth_stencil_view->ReleaseDepthStencilView();
	}

	if (m_p_swap_chain != nullptr)
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

	//RenderTarget 관련 자원 생성
	CreateRenderTargetView();

	//Post Effect Target Texture 생성
	RenderManager::GetInstance()->ResizePostEffectTexture();

	//DepthStencil 관련 자원 생성
	CreateDepthStencilView();

	//Viewport 재설정
	SetViewport(width, height);

	auto settings = Core::GetInstance()->GetSettings();
	std::cout << "Resolution: " << settings->GetWindowWidth() << "X" << settings->GetWindowHeight() << std::endl;

	g_cbuffer_program.resolution = Vector2(static_cast<float>(settings->GetWindowWidth()), static_cast<float>(settings->GetWindowHeight()));
}

void GraphicsManager::SetFullScreen(const bool& is_full_screen)
{
	if (m_p_swap_chain)
		m_p_swap_chain->SetFullscreenState(is_full_screen, nullptr);
}

void GraphicsManager::SetViewport(const UINT& width, const UINT& height)
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


void GraphicsManager::ClearRenderTarget()
{
	auto p_render_target_view = m_p_render_target_view->GetRenderTargetView();
	auto p_depth_stencil_view = m_p_depth_stencil_view->GetDepthStencilView();

	if (m_p_device_context && p_render_target_view && p_depth_stencil_view)
	{
		//백 버퍼(render_target_view)에 그려진 내용 지우기
		m_p_device_context->ClearRenderTargetView(p_render_target_view, Vector4::Black);
		//깊이 버퍼 내용 지우기
		m_p_device_context->ClearDepthStencilView(p_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void GraphicsManager::SetRenderTarget()
{
	auto p_render_target_view = m_p_render_target_view->GetRenderTargetView();
	auto p_depth_stencil_view = m_p_depth_stencil_view->GetDepthStencilView();

	if (m_p_device_context && p_render_target_view && p_depth_stencil_view)
	{
		//백 버퍼에 그려진 내용(render_target_view)을 Output_Merger의 렌더타겟으로 설정
		m_p_device_context->OMSetRenderTargets(1, &p_render_target_view, p_depth_stencil_view);
		//설정한 뷰포트 등록
		m_p_device_context->RSSetViewports(1, &m_viewport);
	}
}

void GraphicsManager::Present()
{
	if (m_p_swap_chain)
	{
		auto settings = Core::GetInstance()->GetSettings();

		//백 버퍼에 그린 내용을 전면 버퍼로 스왑
		auto hResult = m_p_swap_chain->Present(settings->IsVsync() ? 1 : 0, 0);
		assert(SUCCEEDED(hResult));
	}
}

void GraphicsManager::CreateDeviceAndDeviceContext()
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
		m_p_device.GetAddressOf(),
		&feature_levels[1],
		m_p_device_context.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));

	//어댑터가 지원하는 품질 수준 제공
	UINT iMultiSampleLevel = 0;

	m_p_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iMultiSampleLevel);
	assert(iMultiSampleLevel > 0);
}


void GraphicsManager::CreateSwapChain()
{
	auto settings = Core::GetInstance()->GetSettings();

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

	//Get IDXGIAdapter
	p_DXGI_device->GetParent(__uuidof(IDXGIAdapter), (void**)&p_DXGI_adapter);
	assert(SUCCEEDED(hResult));

	//Get IDXGIFactory
	p_DXGI_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&p_DXGI_factory);
	assert(SUCCEEDED(hResult));

	//Create Swap Chain
	p_DXGI_factory->CreateSwapChain(m_p_device.Get(), &desc, &m_p_swap_chain);
	assert(SUCCEEDED(hResult));
}

void GraphicsManager::CreateRenderTargetView()
{
	if (m_p_swap_chain != nullptr)
	{
		//Swap Chain의 백 버퍼의 정보를 받아올 포인터 변수
		//함수 종료 시 자동으로 자원 해제
		ComPtr<ID3D11Texture2D> p_back_buffer = nullptr;

		//Swap Chain의 백 버퍼의 정보를 받아옴
		auto hResult = m_p_swap_chain->GetBuffer
		(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(p_back_buffer.GetAddressOf())
		);
		assert(SUCCEEDED(hResult));

		p_back_buffer->SetPrivateData
		(
			WKPDID_D3DDebugObjectName,
			sizeof("CreateRenderTargetView!!!!!!!!!!!!!!!!!!!!") - 1,
			"CreateRenderTargetView!!!!!!!!!!!!!!!!!!!!"
		);

		if (m_p_render_target_view == nullptr)
		{
			//백 버퍼를 바탕으로 렌더타겟 뷰를 생성
			m_p_render_target_view = ResourceManager::GetInstance()->CreateTexture
			(
				"RenderTargetView",
				p_back_buffer
			);
		}

		else
			m_p_render_target_view->Create(p_back_buffer);
	}
}

void GraphicsManager::CreateDepthStencilView()
{
	if (m_p_device != nullptr)
	{
		auto settings = Core::GetInstance()->GetSettings();

		if (m_p_depth_stencil_view == nullptr)
		{
			m_p_depth_stencil_view = ResourceManager::GetInstance()->CreateTexture
			(
				"DepthStencilView",
				settings->GetWindowWidth(),
				settings->GetWindowHeight(),
				DXGI_FORMAT_D24_UNORM_S8_UINT,
				D3D11_BIND_DEPTH_STENCIL
			);
		}

		else
			m_p_depth_stencil_view->Create
			(
				settings->GetWindowWidth(),
				settings->GetWindowHeight(),
				DXGI_FORMAT_D24_UNORM_S8_UINT,
				D3D11_BIND_DEPTH_STENCIL
			);
	}
}

void GraphicsManager::CreateConstantBuffers()
{
	//WVPMatrix
	auto pair_iter = m_p_constant_buffer_map.insert(std::make_pair(CBuffer_BindSlot::WVPMatrix, std::make_shared<ConstantBuffer>()));
	auto result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create<CBuffer_WVPMatrix>(static_cast<UINT>(CBuffer_BindSlot::WVPMatrix));
	}

	//Material
	pair_iter = m_p_constant_buffer_map.insert(std::make_pair(CBuffer_BindSlot::Material, std::make_shared<ConstantBuffer>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create<CBuffer_Material>(static_cast<UINT>(CBuffer_BindSlot::Material));
	}

	//Program
	pair_iter = m_p_constant_buffer_map.insert(std::make_pair(CBuffer_BindSlot::Program, std::make_shared<ConstantBuffer>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create<CBuffer_Material>(static_cast<UINT>(CBuffer_BindSlot::Program));
	}

	//Light2D
	pair_iter = m_p_constant_buffer_map.insert(std::make_pair(CBuffer_BindSlot::Light2D, std::make_shared<ConstantBuffer>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create<CBuffer_Light2D>(static_cast<UINT>(CBuffer_BindSlot::Light2D));
	}

	//Animation2D
	pair_iter = m_p_constant_buffer_map.insert(std::make_pair(CBuffer_BindSlot::Animation2D, std::make_shared<ConstantBuffer>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create<CBuffer_Animation2D>(static_cast<UINT>(CBuffer_BindSlot::Animation2D));
	}
}

void GraphicsManager::CreateRasterizer()
{
	//Cull Back Solid
	auto pair_iter = m_p_rasterizer_map.insert(std::make_pair(RasterizerType::Cull_Back_Solid, std::make_shared<RasterizerState>()));
	auto result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create
		(
			D3D11_CULL_BACK,
			D3D11_FILL_SOLID
		);
	}

	//Cull Front Solid
	pair_iter = m_p_rasterizer_map.insert(std::make_pair(RasterizerType::Cull_Front_Solid, std::make_shared<RasterizerState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create
		(
			D3D11_CULL_FRONT,
			D3D11_FILL_SOLID
		);
	}

	//Cull None Solid
	pair_iter = m_p_rasterizer_map.insert(std::make_pair(RasterizerType::Cull_None_Solid, std::make_shared<RasterizerState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create
		(
			D3D11_CULL_NONE,
			D3D11_FILL_SOLID
		);
	}

	//Cull None WireFrame
	pair_iter = m_p_rasterizer_map.insert(std::make_pair(RasterizerType::Cull_None_WireFrame, std::make_shared<RasterizerState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create
		(
			D3D11_CULL_NONE,
			D3D11_FILL_WIREFRAME
		);
	}
}

void GraphicsManager::CreateSampler()
{
	//Sampler1
	auto pair_iter = m_p_sampler_map.insert(std::make_pair("Sampler1", std::make_shared<SamplerState>()));
	auto result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create
		(
			0,
			D3D11_FILTER::D3D11_FILTER_ANISOTROPIC,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP
		);
	}

	//Sampler2
	pair_iter = m_p_sampler_map.insert(std::make_pair("Sampler2", std::make_shared<SamplerState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create
		(
			1,
			D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP
		);
	}

	//Sampler3
	pair_iter = m_p_sampler_map.insert(std::make_pair("Sampler3", std::make_shared<SamplerState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create
		(
			2,
			D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP
		);
	}

	//Set Sampler
	ID3D11SamplerState* sampler_array[3]
	{
	   m_p_sampler_map["Sampler1"]->GetSamplerState(),
	   m_p_sampler_map["Sampler2"]->GetSamplerState(),
	   m_p_sampler_map["Sampler3"]->GetSamplerState(),
	};

	m_p_device_context->VSSetSamplers(0, static_cast<UINT>(m_p_sampler_map.size()), sampler_array);
	m_p_device_context->HSSetSamplers(0, static_cast<UINT>(m_p_sampler_map.size()), sampler_array);
	m_p_device_context->DSSetSamplers(0, static_cast<UINT>(m_p_sampler_map.size()), sampler_array);
	m_p_device_context->GSSetSamplers(0, static_cast<UINT>(m_p_sampler_map.size()), sampler_array);
	m_p_device_context->PSSetSamplers(0, static_cast<UINT>(m_p_sampler_map.size()), sampler_array);
	m_p_device_context->CSSetSamplers(0, static_cast<UINT>(m_p_sampler_map.size()), sampler_array);
}

void GraphicsManager::CreateDepthStencil()
{
	//Less DepthStencilState(default)
	auto pair_iter = m_p_depth_stecil_map.insert(std::make_pair(DepthStencilType::Less, std::make_shared<DepthStencilState>()));
	auto result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create(true, D3D11_COMPARISON_LESS);
	}

	//Less_Equal DepthStencilState
	pair_iter = m_p_depth_stecil_map.insert(std::make_pair(DepthStencilType::Less_Equal, std::make_shared<DepthStencilState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create(true, D3D11_COMPARISON_LESS_EQUAL);
	}

	//Grater DepthStencilState
	pair_iter = m_p_depth_stecil_map.insert(std::make_pair(DepthStencilType::Grater, std::make_shared<DepthStencilState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create(true, D3D11_COMPARISON_GREATER);
	}

	//No Test DepthStencilState
	pair_iter = m_p_depth_stecil_map.insert(std::make_pair(DepthStencilType::No_Test, std::make_shared<DepthStencilState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create(false, D3D11_COMPARISON_LESS);
	}

	//No Write DepthStencilState
	pair_iter = m_p_depth_stecil_map.insert(std::make_pair(DepthStencilType::No_Write, std::make_shared<DepthStencilState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create(true, D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ZERO);
	}

	//No Test No Write DepthStencilState
	pair_iter = m_p_depth_stecil_map.insert(std::make_pair(DepthStencilType::No_Test_No_Write, std::make_shared<DepthStencilState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create(false, D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ZERO);
	}
}

void GraphicsManager::CreateBlender()
{
	//Default Blender
	auto pair_iter = m_p_blender_map.insert(std::make_pair(BlendType::Default, std::make_shared<BlendState>()));
	auto result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create(false);
	}

	//Alpha Blender
	pair_iter = m_p_blender_map.insert(std::make_pair(BlendType::Alpha_Blend, std::make_shared<BlendState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create(true, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_COLOR, D3D11_BLEND_OP_ADD, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA);
	}

	//One One Blender
	pair_iter = m_p_blender_map.insert(std::make_pair(BlendType::One_One, std::make_shared<BlendState>()));
	result = pair_iter.second;
	assert(result);
	if (result)
	{
		pair_iter.first->second->Create(true, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	}
}

const std::shared_ptr<ConstantBuffer>& GraphicsManager::GetConstantBuffer(const CBuffer_BindSlot& bind_slot)
{
	auto map_iter = m_p_constant_buffer_map.find(bind_slot);
	auto result = (map_iter != m_p_constant_buffer_map.end());
	assert(result);
	if (result)
	{
		return map_iter->second;
	}

	return nullptr;
}

const std::shared_ptr<RasterizerState>& GraphicsManager::GetRasterizer(const RasterizerType& rasterizer_type)
{
	auto map_iter = m_p_rasterizer_map.find(rasterizer_type);
	auto result = (map_iter != m_p_rasterizer_map.end());
	assert(result);
	if (result)
	{
		return map_iter->second;
	}

	return nullptr;
}

const std::shared_ptr<DepthStencilState>& GraphicsManager::GetDepthStencilState(const DepthStencilType& depth_stencil_type)
{
	auto map_iter = m_p_depth_stecil_map.find(depth_stencil_type);
	auto result = (map_iter != m_p_depth_stecil_map.end());
	assert(result);
	if (result)
	{
		return map_iter->second;
	}

	return nullptr;
}

const std::shared_ptr<BlendState>& GraphicsManager::GetBlender(const BlendType& blend_type)
{
	auto map_iter = m_p_blender_map.find(blend_type);
	auto result = (map_iter != m_p_blender_map.end());
	assert(result);
	if (result)
	{
		return map_iter->second;
	}

	return nullptr;
}