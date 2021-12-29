#pragma once

#define NOMINMAX
#include <Windows.h>

#include "WindowSettings/resource.h"

#include <assert.h>

//정적 라이브러리 헤더 연결
#include <DX11_2D_GameEngine_Lib/stdafx.h>

//DX11_2D_GameEngine_Lib 연결
#ifdef _DEBUG //디버그 모드일 때
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Debug/DX11_2D_GameEngine_Lib_Debug.lib")
#include <dxgidebug.h>
#else //릴리즈 모드일 때
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Release/DX11_2D_GameEngine_Lib_Release.lib")
#endif

//Script_Lib 연결
#ifdef _DEBUG //디버그 모드일 때
#pragma comment(lib, "Script_Lib/Debug/Script_Lib_Debug.lib")
#include <dxgidebug.h>
#else //릴리즈 모드일 때
#pragma comment(lib, "Script_Lib/Release/Script_Lib_Release.lib")
#endif

//Imgui Header
#pragma region imgui
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include "Win32/imgui_impl_win32.h"
#include "DirectX11/imgui_impl_dx11.h"
#pragma endregion

//Memory Leak Check
#ifdef _DEBUG //디버그 모드일 때
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h> // 메모리 할당 및 할당 해제를 추적하는 함수들이 정의되어 있음.
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

//Enum
#include "Engine_Enum.h"

//Define
#include "Engine_Define.h"

//Func
#include "GUI/GUI_Function.h"

//Drag&Drop
#include "Helper/DragDropEvent.h"

#include <limits>