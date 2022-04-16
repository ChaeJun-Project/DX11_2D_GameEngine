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
   const std::string& GetCurrentTime_str() { return m_current_time; }

   void Wait();

private:
   void CalcCurrentTime();
   void CalcFps();

private:
   __int64 m_current_count;
   __int64 m_previous_count;
   __int64 m_frequency_count;

   double m_delta_time = 0.0;		 //프레임 사이의 시간 차이
   double m_accumulate_time = 0.0;	 //1초 체크를 위한 누적 시간

   UINT m_func_call_count = 0; //함수 호출 회수 체크
   UINT m_fps = 0; //초당 호출 횟수

   UINT m_fixed_fps = 0; //고정 프레임

   bool m_is_wait = false;

   std::string m_current_time;
   std::string m_render_str;
};