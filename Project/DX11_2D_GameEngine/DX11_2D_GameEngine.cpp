#include "stdafx.h"
#include "Window/Window.h"

#include "Scene/ClientSceneManager.h"
#include "Editor/Manager/EditorManager.h"
#include "Editor/Manager/EditorObjectManager.h"

void D3D11Debug();

LPCWSTR g_class_name = L"DX11_2D_GameEngine";

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

	Window::Create(hInstance, 1600, 900, g_class_name, IDI_ZERO, false);
	Window::Show(nCmdShow);

	//Core
	CORE->Initialize();

	ClientSceneManager::Initialize();

	CORE->PostInitialize();

	//Scene Manager
	SCENE_MANAGER->SetClientState(static_cast<UINT>(client_state));

	if (SCENE_MANAGER->GetClientState() == static_cast<UINT>(ClientState::Game))
	{
		//Load Engine Data(Content/Engine)
		FileFunction::LoadPhysics((FILE_MANAGER->GetAbsoluteContentPath() + "Engine/Physics.txt"));
		FileFunction::LoadGameResolution((FILE_MANAGER->GetAbsoluteContentPath() + "Engine/Resolution.txt"));

		//Game Title
		auto game_title_scene = SCENE_MANAGER->LoadScene((FILE_MANAGER->GetAbsoluteContentPath() + "Asset/Scene/Game Title.scene"));
		SCENE_MANAGER->SetCurrentScene(game_title_scene);
	}

	else
	{
		//Editor
		ClientSceneManager::CreateNewScene();

		//Editor Manager
		//Initialize Editor Manager
		EDITOR_MANAGER->Initialize(SETTINGS->GetWindowHandle(), GRAPHICS_MANAGER->GetDevice(), GRAPHICS_MANAGER->GetDeviceContext());

		//EditorObject Manager
		EDITOR_OBJECT_MANAGER->Initialize();

		//Window Event 등록	
		//Editor Event
		Window::editor_input_event = ImGui_ImplWin32_WndProcHandler;
		//Window Resize Event
		auto editor_manager = EDITOR_MANAGER;
		Window::resize_event = [&editor_manager](const UINT& width, const UINT& height)
		{
			editor_manager->ResizeEditor(width, height);
		};
	}

	//Mouse Event
	Window::user_input_event = INPUT_MANAGER->MouseProc;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_ZERO));

	while (Window::Update(hAccelTable))
	{
		CORE->Progress();

		//현재 Client가 Editor 상태인 경우 
		if (client_state == ClientState::Editor)
		{
			EDITOR_OBJECT_MANAGER->Update();
			EDITOR_OBJECT_MANAGER->Render();

			EDITOR_MANAGER->Update();
			EDITOR_MANAGER->Render();
		}

		//Graphics Swap Chain Present
		//Imgui의 내용도 포함해서 그린 화면을 전면 버퍼로 스왑해야하기 때문에
		//현재의 시점에서 스왑
		GRAPHICS_MANAGER->Present();
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