#pragma once
#include "stdafx.h"

//namespace�� ����ϴ� ����: �ߺ������� ����� �浹 ����
namespace Window
{
	//HINSTANCE: ���α׷��� ���� �޸𸮸� ������ �ִ�
	//���α׷��� �ν��Ͻ� �ĺ���(�ڵ�)
	//�����찡 �������� ���α׷����� �����ϱ� ���� �ĺ���
	static HINSTANCE g_instance;

	//HWND: ���α׷� �������� ������ �ĺ���(�ڵ�)
	//�ϳ��� ���α׷��� ���� �����츦 ���� �� �ֱ� ������ �ʿ�
	static HWND g_handle;

	// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(g_instance, MAKEINTRESOURCE(IDD_ABOUTBOX), handle, About);
				break;
			case IDM_EXIT:
				DestroyWindow(handle);
				break;
			default:
				return DefWindowProc(handle, message, wParam, lParam);
			}
		}
		break;
		case WM_DISPLAYCHANGE:
		case WM_SIZE:

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
	inline void Create(HINSTANCE instance, const unsigned int& width, const unsigned int& height, LPCWSTR class_name)
	{
		g_instance = instance;
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = g_instance;
		wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_WINDOWICON));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = class_name;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_WINDOWICON));

		//CreateWindowExW �Լ��� ȣ�� �� �� ����� â Ŭ������ ��� 
		auto check = RegisterClassEx(&wcex);
		assert(check != 0);

		//�ʱ�ȭ�� ������ â Ŭ������ ������ ���� ������â ����
		g_handle = CreateWindowW
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
			instance,
			nullptr
		);

		assert(g_handle != nullptr);

		RECT rect{ 0,0,static_cast<LONG>(width),static_cast<LONG>(height) };

		//GetSystemMetrics : ��ǻ�� �ϵ��� ���� �޶����ų� ����ڿ� ���ؼ� �缳�� ������ ��������
		//���α׷����� �� ����ϰ� ���� �� ���
		//SM_CXSCREEN: ���� ȭ�� �ػ��� x�� ũ��(pixel����)�� ����
		//SM_CYSCREEN: ���� ȭ�� �ػ��� y�� ũ��(pixel����)�� ����
		unsigned int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		unsigned int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE); //�������� ũ�⸦ �����ϰ� �����츦 ����� ��ũ�� ��(�����ʿ� �ִ� ��)�� ���ŵ� ������ �����찡 �������.

		MoveWindow(g_handle, x, y, rect.right - rect.left, rect.bottom - rect.top, FALSE); //������ ���ڰ� FALSE�̸� �����츦 �ٽ� �׸��� ����.
	}

	inline void Show(int nCmdShow)
	{
		//��� â�� �� ������ ���
		SetForegroundWindow(g_handle);
		//��� â�� �� ������ ��� �� ��Ŀ�� ����
		SetFocus(g_handle);
		ShowCursor(TRUE);
		ShowWindow(g_handle, nCmdShow);
		UpdateWindow(g_handle);
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
		DestroyWindow(g_handle);
		UnregisterClass(class_name, g_instance);
	}
}