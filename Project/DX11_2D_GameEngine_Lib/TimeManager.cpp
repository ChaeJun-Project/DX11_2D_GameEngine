#include "stdafx.h"
#include "TimeManager.h"

#include "FontManager.h"

void TimeManager::Initialize()
{
	//�ʴ� ī��Ʈ Ƚ�� �޾ƿ���
	//�ý��� ���� �� �����Ǹ� ��� ���μ������� �ϰ��� ��
	assert(QueryPerformanceFrequency(&m_frequency_count));

	//���� ���� ī���� �� �޾ƿ���
	assert(QueryPerformanceCounter(&m_previous_count));
}

void TimeManager::Update()
{
	QueryPerformanceCounter(&m_current_count);

	//QuadPart: ���� ���� ����Ǿ� �ִ� ����ü �ʵ�
	m_delta_time = static_cast<double>(m_current_count.QuadPart - m_previous_count.QuadPart) / static_cast<double>(m_frequency_count.QuadPart);

	m_previous_count = m_current_count;

	//Delta Time�� 60���������� ����
	if (m_delta_time > (1.0 / 60.0))
		m_delta_time = 1.0 / 60.0;

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
	++m_func_call_count;
	m_accumulate_time += m_delta_time; //�ð� ����

	if (m_accumulate_time >= 1.0)
	{
		m_fps = m_func_call_count;
		m_func_call_count = 0;
		m_accumulate_time = 0.0;

		m_render_str = "FPS: " + std::to_string(m_fps) + " Delta Time: " + std::to_string(m_delta_time);
	}
}
