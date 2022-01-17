#pragma once

typedef void(*PFUNC)(void);

template<typename T>
class Singleton
{
private:
    //���� ������ ���� ����(�޸� �Ҵ� x)
	static T* g_instance;

public:
	static T* GetInstance()
	{
	    //g_instance�� ����Ű�� �޸� �ּҰ� ���ٸ�
		if (g_instance == nullptr)
			g_instance = new T; //�޸� ���� �Ҵ�

		return g_instance;
	}

private:
	static void Destroy()
	{
		//g_instance�� ����Ű�� �޸� �ּҰ� �ִٸ�
		if (g_instance != nullptr)
		{
			delete g_instance; //�޸� ���� ����
			g_instance = nullptr;
		}
	}

protected:
	Singleton()
	{
	    //atexit: ���� �Լ�(�ܼ�: main, ������: WinMain)�� ����� �� �̸� ����� �Լ��� ȣ��
		//LIFO(Last In First Out)�� ������ ������ ����
	    atexit((PFUNC)&Singleton::Destroy);
	}

	virtual ~Singleton() = default;
};

//g_instance �ʱ�ȭ
//g_instance�� ���� ������ �����̱� ������
//��� ��ü�� �����ؾ� �ϹǷ� ���α׷� ��ü �������� �޸𸮰� �����Ǿ�� ��
//���� �ݵ�� ���� �������� ���� �� �ʱ�ȭ�� �ؾ� ��
//���� ������ ���� ����(�޸� �Ҵ� o)
template<typename T>
T* Singleton<T>::g_instance = nullptr;