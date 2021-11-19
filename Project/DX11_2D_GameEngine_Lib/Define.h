#pragma once

//Unsigned typedef
typedef unsigned long ULONG;

//싱글톤 매크로 정의
//Singleton를 상속받는 type 클래스의 모든 멤버에 접근하여 사용하기 위해 friend 사용
#define SINGLETON(type) private:\
						friend class Singleton<type>;\


//메모리 해제 관련 매크로 정의
#define SAFE_DELETE(p)			{ if(p) { delete p; p = nullptr; } }
#define SATE_DELETE_ARRAY(p)	{ if(p) { delete[] p; p = nullptr;} }

//자원 정보 받기
#define SAFE_GET_POINTER(p)       { if(p) {return p;} return nullptr; }

//DirectX11 
//자원 메모리 해제 매크로 정의
#define SAFE_RELEASE(p)         { if(p) { p->Release(); p = nullptr; } }

//Key 상태 체크 매크로 정의 
#define KEY_PRESS(key) InputManager::GetInstance()->KeyPress(key)
#define KEY_DOWN(key) InputManager::GetInstance()->KeyDown(key)
#define KEY_UP(key) InputManager::GetInstance()->KeyUp(key)

//Mouse 위치 매크로 정의


//클래스 복사 생성 매크로 정의(깊은 복사, 기존 클래스와 클론 클래스가 같은 메모리를 참조하지 않음)
#define CLONE(type) type* Clone() {return new type(*this);}

