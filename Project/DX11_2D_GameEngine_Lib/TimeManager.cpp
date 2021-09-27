#include "stdafx.h"
#include "TimeManager.h"

TimeManager::TimeManager()
{
	Initialize();
}

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

//Debug ��忡���� Delta Time�� ����
#ifdef _DEBUG
    if(m_delta_time > (1.0 / 60.0))
		m_delta_time = 1.0 / 60.0;
#endif
}

void TimeManager::Render()
{
     ++m_func_call_count;
	 m_accumulate_time += m_delta_time; //�ð� ����

	 if (m_accumulate_time > 1.0)
	 {
		 m_fps = m_func_call_count;
		 m_func_call_count = 0;
		 m_accumulate_time = 0.0;
	 }
}
