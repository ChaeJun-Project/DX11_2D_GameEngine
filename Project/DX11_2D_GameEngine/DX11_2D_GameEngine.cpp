#include "stdafx.h"
#include <DX11_2D_GameEngine_Lib/Window.h>
#include <DX11_2D_GameEngine_Lib/Core.h>
#include <DX11_2D_GameEngine_Lib/InputManager.h>
#include <DX11_2D_GameEngine_Lib/GraphicsManager.h>

void D3D11Debug();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	//메모리 누수 체크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto core = Core::GetInstance();

	Window::Create(hInstance, 1280, 800, L"DX11_2D_GameEngine", IDI_ZERO, false);
	Window::Show(nCmdShow);

	core->Initialize();

	auto graphics_manager = GraphicsManager::GetInstance();

	//Window Event 등록
	Window::user_input_event = InputManager::GetInstance()->MouseProc;
	Window::resize_event = [&graphics_manager](const UINT& width, const UINT& height)
	{
		graphics_manager->ResizeWindowByUser(width, height);
	};

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_ZERO));

	while (Window::Update(hAccelTable))
	{
		core->Progress();
	}

#ifdef _DEBUG //디버그 모드일 때
	D3D11Debug();
#endif
	return 0;
}

void D3D11Debug()
{
     HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	 decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));

	 IDXGIDebug* debug;

	 GetDebugInterface(IID_PPV_ARGS(&debug));

	 OutputDebugStringW(L"ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ\r\n");
	 debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
	 OutputDebugStringW(L"ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ\r\n");

	 debug->Release();
}