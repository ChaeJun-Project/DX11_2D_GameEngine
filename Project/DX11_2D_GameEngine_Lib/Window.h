#pragma once
#include "stdafx.h"

//namespace�� ����ϴ� ����: �ߺ������� ����� �浹 ����
namespace Window
{
	static std::function<void(const UINT&, const UINT&)> resize_event;

	//���� ��ȭ ������ �޽��� ó�����Դϴ�.
	inline INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}

	//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
	//  WM_PAINT    - �� â�� �׸��ϴ�.
	//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
	inline LRESULT CALLBACK WndProc(HWND handle, unsigned int message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// �޴� ������ ���� �м��մϴ�:
			/*switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(g_instance, MAKEINTRESOURCE(IDD_ABOUTBOX), handle, About);
				break;
			case IDM_EXIT:
				DestroyWindow(handle);
				break;
			default:
				return DefWindowProc(handle, message, wParam, lParam);
			}*/
		}
		break;
		case WM_DISPLAYCHANGE:
		case WM_SIZE:
			//�̺�Ʈ �Լ��� ��ϵǾ� �ִ� ���
			if (resize_event && wParam != SIZE_MINIMIZED)
			{
				auto setting = Settings::GetInstance();

				setting->SetWindowWidth(LOWORD(lParam));
				setting->SetWindowHeight(HIWORD(lParam));

				resize_event(LOWORD(lParam), HIWORD(lParam));
			}
			break;

		case WM_CLOSE:
		case WM_DESTROY:
			//���� �����찡 �ı��Ǹ� ���� ���α׷��� ����
			PostQuitMessage(0);
			break;

		default:
			//ó������ ���� ��� �޽����� ����Ʈ ó���� ����
			//PostQuitMessage�� ȣ���� ���� ����
			return DefWindowProc(handle, message, wParam, lParam);
		}

		return 0;
	}

	//������ â ����
	inline void Create(HINSTANCE program_instance, const UINT& width, const UINT& height, LPCWSTR class_name, UINT window_icon, UINT window_icon_sm, const bool& is_full_screen)
	{
		auto setting = Settings::GetInstance();

		//���α׷� �ּ� ����
		setting->SetProgramInstance(program_instance);

		//Create Window Class
		//================================================================================================
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = program_instance;
		wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(window_icon)); //IDI_WINDOWICON2
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = class_name;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(window_icon_sm));

		//CreateWindowExW �Լ��� ȣ�� �� �� ����� â Ŭ������ ��� 
		auto check = RegisterClassEx(&wcex);
		assert(check != 0);
		//================================================================================================

		//Create Window
		//================================================================================================
		//�ʱ�ȭ�� ������ â Ŭ������ ������ ���� ������â ����
		auto window_handle = CreateWindowW
		(
			wcex.lpszClassName,
			wcex.lpszClassName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			nullptr,
			nullptr,
			setting->GetProgramInstance(),
			nullptr
		);

		assert(window_handle != nullptr);
		//================================================================================================

		//������ �ڵ� ����
		setting->SetWindowHandle(window_handle);

		//Set Window Size
		//================================================================================================
		//������ ������ ����
		setting->SetWindowWidth(width); //�ʺ�
		setting->SetWindowHeight(height); //����
		setting->SetFullScreen(is_full_screen); //��üȭ�� ���� ����

		RECT rect{ 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };

		//GetSystemMetrics : ��ǻ�� �ϵ��� ���� �޶����ų� ����ڿ� ���ؼ� �缳�� ������ ��������
		//���α׷����� �� ����ϰ� ���� �� ���
		//SM_CXSCREEN: ���� ȭ�� �ػ��� x�� ũ��(pixel����)�� ����
		//SM_CYSCREEN: ���� ȭ�� �ػ��� y�� ũ��(pixel����)�� ����
		UINT x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		UINT y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE); //�������� ũ�⸦ �����ϰ� �����츦 ����� ��ũ�� ��(�����ʿ� �ִ� ��)�� ���ŵ� ������ �����찡 �������.

		MoveWindow(setting->GetWindowHandle(), x, y, rect.right - rect.left, rect.bottom - rect.top, FALSE); //������ ���ڰ� FALSE�̸� �����츦 �ٽ� �׸��� ����.
	}

	inline void Show(int nCmdShow)
	{
		auto window_handle = Settings::GetInstance()->GetWindowHandle();

		//��� â�� �� ������ ���
		SetForegroundWindow(window_handle);
		//��� â�� �� ������ ��� �� ��Ŀ�� ����
		SetFocus(window_handle);
		ShowCursor(TRUE);
		ShowWindow(window_handle, nCmdShow);
		UpdateWindow(window_handle);
	}

	inline const bool Update(HACCEL hAccelTable)
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		//GetMessage - �޼����� �ö����� ��ٸ�(�ٸ��� ó������ ����) (����)
		//PeekMessage - �޼����� �ȵ����� �׳� �Ѿ (�񵿱�)

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				//�޼��� ����
				TranslateMessage(&msg);
				//WndProc���� �޼��� ����
				DispatchMessage(&msg);
			}
		}

		//WM_QUIT�� PostQuitMessage(0)���� ������
		return msg.message != WM_QUIT;
	}

	inline void Destroy(LPCWSTR class_name)
	{
		auto setting = Settings::GetInstance();
		DestroyWindow(setting->GetWindowHandle());
		UnregisterClass(class_name, setting->GetProgramInstance());
	}
}