#include "stdafx.h"
#include "GUI_Device.h"

#define INDENT 120.0f

GUI_Device::GUI_Device(const std::string& device_title)
	:IGUI(device_title)
{
}

void GUI_Device::Initialize()
{
	//CPU
	m_str_cpu_name = SETTINGS->GetCPUName();
	m_str_core_count = std::to_string(SETTINGS->GetCoreCount());

	//Ram(GB)
	m_str_ram_memory = std::to_string(SETTINGS->GetRamMemory());
	m_str_ram_memory += "GB";

	//GPU
	m_str_gpu_name = SETTINGS->GetGPUName();
	m_str_video_ram_memory = std::to_string(SETTINGS->GetVRamMemory());
	m_str_video_ram_memory += "GB";

	//Screen Rate(스크린 주사율)
	m_str_screen_rate = std::to_string(SETTINGS->GetSreenRate());
}

void GUI_Device::Render()
{
	//CPU
	ImGui::BulletText("CPU: ");
	ImGui::SameLine(INDENT);
	ImGui::Text(m_str_cpu_name.c_str());

	ImGui::BulletText("CPU Core: ");
	ImGui::SameLine(INDENT);
	ImGui::Text(m_str_core_count.c_str());

	//Ram(GB)
	ImGui::BulletText("Ram Memory: ");
	ImGui::SameLine(INDENT);
	ImGui::Text(m_str_ram_memory.c_str());

	//GPU
	ImGui::BulletText("GPU: ");
	ImGui::SameLine(INDENT);
	ImGui::Text(m_str_gpu_name.c_str());
	
	ImGui::BulletText("VRam Memory: ");
	ImGui::SameLine(INDENT);
	ImGui::Text(m_str_video_ram_memory.c_str());

	//Screen Rate(스크린 주사율)
	ImGui::BulletText("Screen Rate: ");
	ImGui::SameLine(INDENT);
	ImGui::Text(m_str_screen_rate.c_str());
}
