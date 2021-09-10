#include "stdafx.h"
#include "Window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    Window::Create(hInstance, 1200, 800, L"DX11_2D_GameEngine");
    Window::Show(nCmdShow);

    HACCEL hAccelTable = LoadAccelerators(Window::g_instance, MAKEINTRESOURCE(IDI_WINDOWICON));

    while (Window::Update(hAccelTable))
    {
       //게임 로직넣기
    }

    return 0;
}