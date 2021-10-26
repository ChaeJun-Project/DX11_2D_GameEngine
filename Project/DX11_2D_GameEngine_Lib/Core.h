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
	void Progress();

public:
	std::shared_ptr<Settings>& GetSettings() { return this->m_p_settings; }

private:
	std::shared_ptr<Settings> m_p_settings = nullptr;
};

