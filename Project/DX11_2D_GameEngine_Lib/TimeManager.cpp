#include "stdafx.h"
#include "TimeManager.h"

#include "FontManager.h"

void TimeManager::Initialize()
{
	//�ʴ� ī��Ʈ Ƚ�� �޾ƿ���
	//LARGE_INTEGER: ũ�Ⱑ ū ������ �ڷ��� Ÿ��(= __int64)
	//�ý��� ���� �� �����Ǹ� ��� ���μ������� �ϰ��� ��(1�ʴ� ������, ������)
	QueryPerformanceFrequency((LARGE_INTEGER*)(&m_frequency_count));
	//���� ���� ī���� �� �޾ƿ���
	QueryPerformanceCounter((LARGE_INTEGER*)(&m_previous_count));

	CalcCurrentTime();
}

void TimeManager::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)(&m_current_count));

	//QuadPart: ���� ���� ����Ǿ� �ִ� ����ü �ʵ�
	//Delta Time = (���� ������ ������ ī��Ʈ - ���� ������ ������ ī��Ʈ) / �ʴ� ������ ī��Ʈ(����)
	m_delta_time = static_cast<double>(m_current_count - m_previous_count) / static_cast<double>(m_frequency_count);

	m_previous_count = m_current_count;

	CalcCurrentTime();
	CalcFps();

	g_cbuffer_program.delta_time = static_cast<float>(m_delta_time);
	g_cbuffer_program.accumulate_time += static_cast<float>(m_delta_time);
}

void TimeManager::Render()
{
	FONT_MANAGER->DrawFont(m_render_str, 20.0f, FONT_RGBA(0, 255, 0, 255), 10.0f, 10.0f);
}

void TimeManager::CalcCurrentTime()
{
	time_t now = time(NULL);

	tm date_time;
	ZeroMemory(&date_time, sizeof(tm));

	date_time = *localtime(&now);

	m_current_time = "[" + std::to_string(date_time.tm_year + 1900) + "/"
		+ std::to_string(date_time.tm_mon + 1) + "/"
		+ std::to_string(date_time.tm_mday) + "/"
		+ std::to_string(date_time.tm_hour) + ":"
		+ std::to_string(date_time.tm_min) + ":"
		+ std::to_string(date_time.tm_sec) + "]";
}

void TimeManager::CalcFps()
{
	//Delta Time�� ���� ���������� ����
	/*if (m_fixed_fps != 0)
		m_delta_time = static_cast<double>(1.0f / m_fixed_fps);*/

	++m_func_call_count;
	m_accumulate_time += m_delta_time; //�ð� ����

	if (m_accumulate_time >= 1.0)
	{
		m_fps = m_func_call_count;
		m_func_call_count = 0;
		m_accumulate_time = 0.0;

		m_is_wait = true;

		m_render_str = "FPS: " + std::to_string(m_fps) + " Delta Time: " + std::to_string(m_delta_time);
	}
}
