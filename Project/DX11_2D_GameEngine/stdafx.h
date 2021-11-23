#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include "WindowSettings/resource.h"

#include <assert.h>

//정적 라이브러리 헤더 연결
#include <DX11_2D_GameEngine_Lib/stdafx.h>

//정적 라이브러리 연결
#ifdef _DEBUG //디버그 모드일 때
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Debug/DX11_2D_GameEngine_Lib_Debug.lib")
#include <dxgidebug.h>
#else //릴리즈 모드일 때
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Release/DX11_2D_GameEngine_Lib_Release.lib")
#endif

//Imgui Header
#pragma region imgui
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Win32/imgui_impl_win32.h"
#include "DirectX11\imgui_impl_dx11.h"
#pragma endregion

//Enum
#include "Engine_Enum.h"

#include <limits>