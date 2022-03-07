#pragma once
#include "stdafx.h"

#include <DX11_2D_GameEngine_Lib/Core.h>
#include <DX11_2D_GameEngine_Lib/Settings.h>

//namespace를 사용하는 이유: 중복때문에 생기는 충돌 방지
namespace Window
{
	static std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> user_input_event;
	static std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> editor_input_event;
	static std::function<void(const UINT&, const UINT&)> resize_event;
	
	//정보 대화 상자의 메시지 처리기입니다.
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

	//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
	//  WM_PAINT    - 주 창을 그립니다.
	//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
	inline LRESULT CALLBACK WndProc(HWND handle, unsigned int message, WPARAM wParam, LPARAM lParam)
	{
	    //유저 입력 이벤트 함수를 등록했다면
		if (user_input_event != nullptr)
		{
			user_input_event(handle, message, wParam, lParam);
		}

		//편집기(ImGui) 입력 이벤트 함수를 등록했다면
		if (editor_input_event != nullptr)
		{
			editor_input_event(handle, message, wParam, lParam);
		}

		switch (message)
		{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 메뉴 선택을 구문 분석합니다:
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
			//이벤트 함수가 등록되어 있는 경우
			if (resize_event && wParam != SIZE_MINIMIZED)
			{
				auto settings = Core::GetInstance()->GetSettings();

				settings->SetWindowWidth(LOWORD(lParam));
				settings->SetWindowHeight(HIWORD(lParam));

				resize_event(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_DPICHANGED:
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
			{
				//const int dpi = HIWORD(wParam);
				//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
				const RECT* suggested_rect = (RECT*)lParam;
				SetWindowPos(handle, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
			}
		case WM_CLOSE:
		case WM_DESTROY:
			//메인 윈도우가 파괴되면 응용 프로그램을 종료
			PostQuitMessage(0);
			break;

		default:
			//처리되지 않은 모든 메시지의 디폴트 처리를 수행
			//PostQuitMessage는 호출해 주지 않음
			return DefWindowProc(handle, message, wParam, lParam);
		}

		return 0;
	}

	//윈도우 창 생성
	inline void Create(HINSTANCE program_instance, const UINT& width, const UINT& height, LPCWSTR class_name, UINT window_icon, const bool& is_full_screen)
	{
		auto settings = Core::GetInstance()->GetSettings();

		//프로그램 주소 저장
		settings->SetProgramInstance(program_instance);

		//Create Window Class
		//================================================================================================
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = program_instance;
		wcex.hIcon = LoadIcon(program_instance, MAKEINTRESOURCE(window_icon)); //IDI_ZERO
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = class_name;
		wcex.hIconSm = LoadIcon(program_instance, MAKEINTRESOURCE(window_icon));

		//CreateWindowExW 함수를 호출 할 때 사용할 창 클래스를 등록 
		auto check = RegisterClassEx(&wcex);
		assert(check != 0);
		//================================================================================================

		//Create Window
		//================================================================================================
		//초기화된 윈도우 창 클래스의 내용을 토대로 윈도우창 생성
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
			settings->GetProgramInstance(),
			nullptr
		);

		assert(window_handle != nullptr);
		//================================================================================================

		//윈도우 핸들 저장
		settings->SetWindowHandle(window_handle);

		//Set Window Size
		//================================================================================================
		//윈도우 사이즈 저장
		settings->SetWindowWidth(width); //너비
		settings->SetWindowHeight(height); //높이
		settings->SetFullScreen(is_full_screen); //전체화면 여부 설정

		RECT rect{ 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };

		//GetSystemMetrics : 컴퓨터 하드웨어에 따라 달라지거나 사용자에 의해서 재설정 가능한 정보들을
		//프로그램에서 얻어서 사용하고 싶을 때 사용
		//SM_CXSCREEN: 현재 화면 해상도의 x축 크기(pixel단위)를 얻음
		//SM_CYSCREEN: 현재 화면 해상도의 y축 크기(pixel단위)를 얻음
		UINT x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		UINT y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE); //윈도우의 크기를 설정하고 윈도우를 만들면 스크롤 바(오른쪽에 있는 바)가 제거된 상태의 윈도우가 만들어짐.

		MoveWindow(settings->GetWindowHandle(), x, y, rect.right - rect.left, rect.bottom - rect.top, FALSE); //마지막 인자가 FALSE이면 윈도우를 다시 그리지 않음.
	}

	inline void Show(int nCmdShow)
	{
		auto window_handle = Core::GetInstance()->GetSettings()->GetWindowHandle();

		//띄운 창을 맨 앞으로 띄움
		SetForegroundWindow(window_handle);
		//띄운 창을 맨 앞으로 띄운 후 포커스 지정
		SetFocus(window_handle);
		ShowCursor(TRUE);
		ShowWindow(window_handle, nCmdShow);
		UpdateWindow(window_handle);
	}

	inline const bool Update(HACCEL hAccelTable)
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		//GetMessage - 메세지가 올때까지 기다림(다른걸 처리하지 않음) (동기)
		//PeekMessage - 메세지가 안들어오면 그냥 넘어감 (비동기)

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				//메세지 생성
				TranslateMessage(&msg);
				//WndProc으로 메세지 전달
				DispatchMessage(&msg);
			}
		}

		//WM_QUIT는 PostQuitMessage(0)에서 설정됨
		return msg.message != WM_QUIT;
	}

	inline void Destroy(LPCWSTR class_name)
	{
		auto settings = Core::GetInstance()->GetSettings();
		DestroyWindow(settings->GetWindowHandle());
		UnregisterClass(class_name, settings->GetProgramInstance());
	}
}