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
#define SAFE_GET_POINT(p)       { if(p) {return p;} return nullptr; }

//DirectX11 
//자원 메모리 해제 매크로 정의
#define SAFE_RELEASE(p)         { if(p) { p->Release(); p = nullptr; } }

//Device, DeviceContext 매크로 정의
#define GET_DEVICE Graphics::GetInstance()->GetDevice();
#define GET_CONTEXT Graphics::GetInstance()->GetDeviceContext();

//Key 상태 체크 매크로 정의 
#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)

//Mouse 위치 매크로 정의
#define MOUSE_POS CKeyMgr::GetInst()->GetMousePos()

//클래스 복사 생성 매크로 정의(깊은 복사, 기존 클래스와 클론 클래스가 같은 메모리를 참조하지 않음)
#define CLONE(type) type* Clone() {return new type(*this);}