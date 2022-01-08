#pragma once

#include "Singleton.h"

class TimeManager final : public Singleton<TimeManager>
{
	SINGLETON(TimeManager);

	TimeManager() = default;
	~TimeManager() = default;

public:
   void Initialize();
   void Update();
   void Render();

public:
   const double& GetDeltaTime_double() { return m_delta_time; }
   const float& GetDeltaTime_float() { return static_cast<float>(m_delta_time); }
   const std::string& GetCurrentTime_str() { return current_time; }

private:
   void CalcCurrentTime();
   void CalcFps();

private:
   //LARGE_INTEGER: 크기가 큰 정수형 자료형 타입(= __int64)
   LARGE_INTEGER m_current_count;
   LARGE_INTEGER m_previous_count;
   LARGE_INTEGER m_frequency_count;

   double m_delta_time = 0.0;		 //프레임 사이의 시간 차이
   double m_accumulate_time = 0.0;	 //1초 체크를 위한 누적 시간

   UINT m_func_call_count = 0; //함수 호출 회수 체크
   UINT m_fps = 0; //초당 호출 횟수

   std::string current_time;
   std::string render_str;
};