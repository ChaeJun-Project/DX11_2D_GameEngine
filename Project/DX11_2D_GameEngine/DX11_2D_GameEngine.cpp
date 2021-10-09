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

    auto s = SceneManager::GetInstance();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_ZERO));

    while (Window::Update(hAccelTable))
    {
        s->Progress();
    }

    return 0;
}