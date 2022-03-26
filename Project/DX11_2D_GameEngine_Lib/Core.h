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
	//Manager 객체의 소멸 순서때문에 따로 작성(SingleTon의 atexit 함수 호출 순서 고려)
	//Manager 객체의 소멸 순서가 SceneManager -> ResourceManager -> AudioManager 이어야 함
	void PostInitialize(); 

	void Progress();

public:
	std::shared_ptr<Settings>& GetSettings() { return m_p_settings; }

private:
	std::shared_ptr<Settings> m_p_settings = nullptr;
};

