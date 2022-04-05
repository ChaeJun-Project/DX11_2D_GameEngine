#pragma once
#include "GUI/IGUI.h"

class GUI_Device final : public IGUI
{
public:
    GUI_Device(const std::string& device_title);
    ~GUI_Device() = default;

public:
    virtual void Initialize() override;
    virtual void Render() override;

private:
	//CPU
	std::string m_str_cpu_name;
	std::string m_str_core_count;

	//Ram(GB)
	std::string m_str_ram_memory;

	//GPU
	std::string m_str_gpu_name;

	//VRam(GB)
	std::string m_str_video_ram_memory;

	//Screen Rate(스크린 주사율)
	std::string m_str_screen_rate;
};

