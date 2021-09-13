// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN         

// Windows 헤더 파일
#include <windows.h>

#include <stdlib.h>
#include <iostream>

//#include <assert.h> //C Ver.
#include <cassert>  //C++ Ver.


#ifdef _DEBUG //디버그 모드일 때 콘솔창 연결

#ifdef UNICODE //유니코드 환경
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else //멀티바이트 환경
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#endif

#include "Global.h"

#endif //PCH_H
