#include "stdafx.h"
#include "Core.h"

#include "Settings.h"

Core::Core()
{
	//Create Settings
	m_p_settings = std::make_shared<Settings>();
}

Core::~Core()
{
	m_p_settings.reset();
}

void Core::Initialize()
{
	//Initialize Device Info
	InitializeDeviceInfo();

    //Initialize File Manager
	FILE_MANAGER->Initialize();
    //Initialize Time Manager 
	TIME_MANAGER->Initialize();
	//Initialize Input Manager
	INPUT_MANAGER->Initialize();
	//Initialize Audio Manager
	AUDIO_MANAGER->Initialize();
	//Initialize Graphics Manager
	GRAPHICS_MANAGER->Initialize();
	//Initialize Scene Manager
	SCENE_MANAGER->Initialize();
	//Initialize Font Manager
	FONT_MANAGER->Initialize();
}

void Core::PostInitialize()
{
	//Initialize Resource Manager
	RESOURCE_MANAGER->Initialize();
	//Initialize Render Manager
	RENDER_MANAGER->Initialize();
}

void Core::Progress()
{
	//Update Time Manager
	TIME_MANAGER->Update();
	//Update Input Manager
	INPUT_MANAGER->Update();
	//Update Scene Manager
	SCENE_MANAGER->Update();
	//Render Render Manager 
	RENDER_MANAGER->Render();
	//Update Audio Manager
	AUDIO_MANAGER->Update();
	//Update Event Manager
	EVENT_MANAGER->Update();
}

void Core::InitializeDeviceInfo()
{
	GetCPUAndRamInfo();
	GetGPUInfo();
}

void Core::GetCPUAndRamInfo()
{
   //현재 프로세스가 사용하고 있는 CPU 및 Ram 정보 불러오기
   //출처: https://stackoverflow.com/questions/850774/how-to-determine-the-hardware-cpu-and-ram-on-a-machine

	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char CPUBrandString[0x40];
	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}

	//CPU Name
	m_p_settings->m_cpu_name = std::string(CPUBrandString);

	//CPU Core
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m_p_settings->m_core_count = static_cast<UINT>(sysInfo.dwNumberOfProcessors);
	
	//Ram Memory
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	m_p_settings->m_ram_memory = static_cast<UINT>(((statex.ullTotalPhys / 1024) / 1024) / 1024);
}

void Core::GetGPUInfo()
{
	//지원하는 해상도 찾기
	//출처: https://copynull.tistory.com/240
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
			m_p_settings->m_numerator = display_mode_list[i].RefreshRate.Numerator;
			m_p_settings->m_denominator = display_mode_list[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC adapter_desc;
	hResult = p_DXGI_adapter->GetDesc(&adapter_desc);

	//GPU Name
	std::wstring gpu_description = adapter_desc.Description;
	m_p_settings->m_gpu_name = FILE_MANAGER->ConvertWStringToString(gpu_description);
	//VRam Memory
	m_p_settings->m_video_ram_memory = static_cast<UINT>(adapter_desc.DedicatedVideoMemory / 1024 / 1024 / 1024);

	//Screen Rate(스크린 주사율)
	m_p_settings->m_screen_rate = m_p_settings->m_numerator / m_p_settings->m_denominator;

	SATE_DELETE_ARRAY(display_mode_list);
}
