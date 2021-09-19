#pragma once

//Unsigned typedef
typedef unsigned long ULONG;

//�̱��� ��ũ�� ����
//Singleton�� ��ӹ޴� type Ŭ������ ��� ����� �����Ͽ� ����ϱ� ���� friend ���
#define SINGLETON(type) private:\
						friend class Singleton<type>;\


//�޸� ���� ���� ��ũ�� ����
#define SAFE_DELETE(p)			{ if(p) { delete p; p = nullptr; } }
#define SATE_DELETE_ARRAY(p)	{ if(p) { delete[] p; p = nullptr;} }

//�ڿ� ���� �ޱ�
#define SAFE_GET_POINT(p)       { if(p) {return p;} return nullptr; }

//DirectX11 
//�ڿ� �޸� ���� ��ũ�� ����
#define SAFE_RELEASE(p)         { if(p) { p->Release(); p = nullptr; } }

//Device, DeviceContext ��ũ�� ����
#define GET_DEVICE Graphics::GetInstance()->GetDevice();
#define GET_CONTEXT Graphics::GetInstance()->GetDeviceContext();

//Key ���� üũ ��ũ�� ���� 
#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)

//Mouse ��ġ ��ũ�� ����
#define MOUSE_POS CKeyMgr::GetInst()->GetMousePos()

//Ŭ���� ���� ���� ��ũ�� ����(���� ����, ���� Ŭ������ Ŭ�� Ŭ������ ���� �޸𸮸� �������� ����)
#define CLONE(type) type* Clone() {return new type(*this);}