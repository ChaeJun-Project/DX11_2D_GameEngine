﻿#pragma once

#define WIN32_LEAN_AND_MEAN 

//정적 라이브러리 헤더 연결
#include <DX11_2D_GameEngine_Lib/stdafx.h>

//정적 라이브러리 연결
#ifdef _DEBUG //디버그 모드일 때
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Debug/DX11_2D_GameEngine_Lib_Debug.lib")
#include <dxgidebug.h>
#else //릴리즈 모드일 때
#pragma comment(lib, "DX11_2D_GameEngine_Lib/Release/DX11_2D_GameEngine_Lib_Release.lib")
#endif

#include <DX11_2D_GameEngine_Lib/Script.h>

