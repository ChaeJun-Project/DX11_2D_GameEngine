#pragma once

#include "Singleton.h"

class Settings;

class Core final : public Singleton<Core>
{ 
	SINGLETON(Core);

	Core();
	~Core();

public:
    void Initialize();
	//Manager ��ü�� �Ҹ� ���������� ���� �ۼ�(SingleTon�� atexit �Լ� ȣ�� ���� ���)
	//Manager ��ü�� �Ҹ� ������ SceneManager -> ResourceManager -> AudioManager �̾�� ��
	void PostInitialize(); 

	void Progress();

public:
	std::shared_ptr<Settings>& GetSettings() { return m_p_settings; }

private:
	std::shared_ptr<Settings> m_p_settings = nullptr;
};

