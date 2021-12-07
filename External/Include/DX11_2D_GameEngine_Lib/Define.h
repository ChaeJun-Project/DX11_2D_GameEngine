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
#define SAFE_GET_POINTER(p)       { if(p) {return p;} return nullptr; }

//DirectX11 
//�ڿ� �޸� ���� ��ũ�� ����
#define SAFE_RELEASE(p)         { if(p) { p->Release(); p = nullptr; } }

//Time
//Delta Time
#define DELTA_TIME_D TimeManager::GetInstance()->GetDeltaTime_double()
#define DELTA_TIME_F TimeManager::GetInstance()->GetDeltaTime_float()

//Input
//Key ���� üũ ��ũ�� ���� 
#define KEY_PRESS(key) InputManager::GetInstance()->KeyPress(key)
#define KEY_DOWN(key) InputManager::GetInstance()->KeyDown(key)
#define KEY_UP(key) InputManager::GetInstance()->KeyUp(key)

//Mouse ���� üũ ��ũ�� ����
#define MOUSE_BUTTON_DOWN(button) InputManager::GetInstance()->BtnDown(button)
#define MOUSE_BUTTON_UP(button) InputManager::GetInstance()->BtnUp(button)
#define MOUSE_BUTTON_PRESS(button) InputManager::GetInstance()->BtnPress(button)

#define MOUSE_MOVE InputManager::GetInstance()->GetMouseMoveValue()

//Ŭ���� ���� ���� ��ũ�� ����(���� ����, ���� Ŭ������ Ŭ�� Ŭ������ ���� �޸𸮸� �������� ����)
#define CLONE(type) type* Clone() {return new type(*this);}

