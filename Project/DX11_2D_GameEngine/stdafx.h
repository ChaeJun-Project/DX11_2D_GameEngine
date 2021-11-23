#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include "WindowSettings/resource.h"

#include <assert.h>

//���� ���̺귯�� ��� ����
#include <DX11_2D_GameEngine_Lib/stdafx.h>

//���� ���̺귯�� ����
#ifdef _DEBUG //����� ����� ��
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Debug/DX11_2D_GameEngine_Lib_Debug.lib")
#include <dxgidebug.h>
#else //������ ����� ��
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