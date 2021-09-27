#include "stdafx.h"
#include "TimeManager.h"

TimeManager::TimeManager()
{
	Initialize();
}

void TimeManager::Initialize()
{
	//초당 카운트 횟수 받아오기
	//시스템 부팅 시 고정되며 모든 프로세서에서 일관된 값
	assert(QueryPerformanceFrequency(&m_frequency_count));

	//현재 성능 카운터 값 받아오기
	assert(QueryPerformanceCounter(&m_previous_count));
}

void TimeManager::Update()
{
	QueryPerformanceCounter(&m_current_count);

	//QuadPart: 실제 값이 저장되어 있는 구조체 필드
	m_delta_time = static_cast<double>(m_current_count.QuadPart - m_previous_count.QuadPart) / static_cast<double>(m_frequency_count.QuadPart);

	m_previous_count = m_current_count;

//Debug 모드에서는 Delta Time을 고정
#ifdef _DEBUG
    if(m_delta_time > (1.0 / 60.0))
		m_delta_time = 1.0 / 60.0;
#endif
}

void TimeManager::Render()
{
     ++m_func_call_count;
	 m_accumulate_time += m_delta_time; //시간 누적

	 if (m_accumulate_time > 1.0)
	 {
		 m_fps = m_func_call_count;
		 m_func_call_count = 0;
		 m_accumulate_time = 0.0;
	 }
}
