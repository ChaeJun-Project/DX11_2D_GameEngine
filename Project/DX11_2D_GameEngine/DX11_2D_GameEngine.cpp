#include "stdafx.h"
#include "Window/Window.h"
#include <DX11_2D_GameEngine_Lib/Core.h>
#include <DX11_2D_GameEngine_Lib/Settings.h>
#include <DX11_2D_GameEngine_Lib/InputManager.h>
#include <DX11_2D_GameEngine_Lib/GraphicsManager.h>
#include "Scene/ClientSceneManager.h"
#include "Editor/Manager/EditorManager.h"
#include "Editor/Manager/EditorObjectManager.h"

void D3D11Debug();

LPCWSTR g_class_name = L"DX11_2D_GameEngine";

//TODO
ClientState client_state = ClientState::Editor;

//Win32를 사용하므로 프로시저 핸들러를 선언해줘야 함.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	//메모리 누수 체크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Core
	auto core = Core::GetInstance();

	Window::Create(hInstance, 1600, 900, g_class_name, IDI_ZERO, false);
	Window::Show(nCmdShow);

	core->Initialize();

	//Settings
	auto settings = core->GetSettings();

	//Graphics Manager
	auto graphics_manager = GraphicsManager::GetInstance();

	//Scene Manager
	auto scene_manager = SceneManager::GetInstance();
	scene_manager->SetClientState(static_cast<UINT>(client_state));

	ClientSceneManager::CreateNewScene();

	//1 Frame Update
	core->Progress();

	//Editor Manager
	auto editor_manager = EditorManager::GetInstance();
	//Initialize Editor Manager
	editor_manager->Initialize(settings->GetWindowHandle(), graphics_manager->GetDevice(), graphics_manager->GetDeviceContext());

	//EditorObject Manager
	auto editor_object_manager = EditorObjectManager::GetInstance();
	//Initialize EditorObject Manager
	editor_object_manager->Initialize();

	//Window Event 등록
	//Mouse Event
	Window::user_input_event = InputManager::GetInstance()->MouseProc;
	//Editor Event
	Window::editor_input_event = ImGui_ImplWin32_WndProcHandler;
	//Window Resize Event
	Window::resize_event = [&editor_manager](const UINT& width, const UINT& height)
	{
		editor_manager->ResizeEditor(width, height);
	};

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_ZERO));

	while (Window::Update(hAccelTable))
	{
		core->Progress();

		switch (client_state)
		{
		case ClientState::Title:
			break;
		case ClientState::Game:
			break;
		case ClientState::Editor:
			editor_object_manager->Update();
			editor_object_manager->Render();

			editor_manager->Update();
		    editor_manager->Render();
			break;
		}

		//Graphics Swap Chain Present
		//Imgui의 내용도 포함해서 그린 화면을 전면 버퍼로 스왑해야하기 때문에
		//현재의 시점에서 스왑
		graphics_manager->EndScene();
	}

	Window::Destroy(g_class_name);

#ifdef _DEBUG //디버그 모드일 때
	D3D11Debug();
#endif
	return 0;
}

void D3D11Debug()
{
#ifdef _DEBUG //디버그 모드일 때
	HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));

	IDXGIDebug* debug;

	GetDebugInterface(IID_PPV_ARGS(&debug));

	OutputDebugStringW(L"ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ\r\n");
	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
	OutputDebugStringW(L"ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ\r\n");

	debug->Release();
#endif
}