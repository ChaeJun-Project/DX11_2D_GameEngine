#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include "resource.h"

#include <assert.h>

//���� ���̺귯�� ��� ����
#include <DX11_2D_GameEngine_Lib/stdafx.h>

//���� ���̺귯�� ����
#ifdef _DEBUG //����� ����� ��
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Debug/DX11_2D_GameEngine_Lib_Debug.lib")
#else //������ ����� ��
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Release/DX11_2D_GameEngine_Lib_Release.lib")
#endif
