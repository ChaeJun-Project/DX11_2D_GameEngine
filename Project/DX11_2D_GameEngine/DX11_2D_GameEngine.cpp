#include "stdafx.h"
#include <DX11_2D_GameEngine_Lib/InputManager.h>
#include <DX11_2D_GameEngine_Lib/Window.h>
#include <DX11_2D_GameEngine_Lib/GraphicsManager.h>
#include <DX11_2D_GameEngine_Lib/SceneManager.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	//메모리 누수 체크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Window::Create(hInstance, 800, 600, L"DX11_2D_GameEngine", IDI_ZERO, false);
	Window::Show(nCmdShow);

	auto d = GraphicsManager::GetInstance();

	Window::user_input_event = InputManager::GetInstance()->MouseProc;
	Window::resize_event = [&d](const UINT& width, const UINT& height)
	{
		d->ResizeWindowByUser(width, height);
	};

	d->Initialize();
	d->CreateConstantBuffers();
	d->CreateSampler();
	d->CreateBlender();

	auto s = SceneManager::GetInstance();

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_ZERO));

	while (Window::Update(hAccelTable))
	{
		s->Progress();
	}

	return 0;
}