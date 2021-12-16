#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include "WindowSettings/resource.h"

#include <assert.h>

//���� ���̺귯�� ��� ����
#include <DX11_2D_GameEngine_Lib/stdafx.h>

//DX11_2D_GameEngine_Lib ����
#ifdef _DEBUG //����� ����� ��
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Debug/DX11_2D_GameEngine_Lib_Debug.lib")
#include <dxgidebug.h>
#else //������ ����� ��
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Release/DX11_2D_GameEngine_Lib_Release.lib")
#endif

//Script_Lib ����
#ifdef _DEBUG //����� ����� ��
#pragma comment(lib, "Script_Lib/Debug/Script_Lib_Debug.lib")
#include <dxgidebug.h>
#else //������ ����� ��
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

//Enum
#include "Engine_Enum.h"

//Define
#include "Engine_Define.h"

//Func
#include "GUI/GUI_Function.h"

//Drag&Drop
#include "Helper/DragDropEvent.h"

#include <limits>