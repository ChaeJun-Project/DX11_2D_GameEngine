#pragma once

typedef void(*PFUNC)(void);

template<typename T>
class Singleton
{
private:
    //정적 포인터 변수 선언(메모리 할당 x)
	static T* g_instance;

public:
	static T* GetInstance()
	{
	    //g_instance가 가리키는 메모리 주소가 없다면
		if (g_instance == nullptr)
			g_instance = new T; //메모리 동적 할당

		return g_instance;
	}

private:
	static void Destroy()
	{
		//g_instance가 가리키는 메모리 주소가 있다면
		if (g_instance != nullptr)
		{
			delete g_instance; //메모리 동적 해제
			g_instance = nullptr;
		}
	}

protected:
	Singleton()
	{
	    //atexit: 메인 함수(콘솔: main, 윈도우: WinMain)가 종료될 때 미리 등록한 함수를 호출
		//LIFO(Last In First Out)의 구조를 가지고 있음
	    atexit((PFUNC)&Singleton::Destroy);
	}

	virtual ~Singleton() = default;
};

//g_instance 초기화
//g_instance는 정적 포인터 변수이기 때문에
//모든 객체가 공유해야 하므로 프로그램 전체 영역에서 메모리가 유지되어야 함
//따라서 반드시 전역 범위에서 정의 및 초기화를 해야 함
//정적 포인터 변수 선언(메모리 할당 o)
template<typename T>
T* Singleton<T>::g_instance = nullptr;