#pragma once

#include "Singleton.h"

class TimeManager final : public Singleton<TimeManager>
{
	SINGLETON(TimeManager);

	TimeManager();
	~TimeManager() = default;

public:
   void Initialize();
   void Update();
   void Render();

public:
   const double& GetDeltaTime_double() { return this->m_delta_time; }
   const float& GetDeltaTime_float() { return static_cast<float>(this->m_delta_time); }

private:
   //LARGE_INTEGER: ũ�Ⱑ ū ������ �ڷ��� Ÿ��(= __int64)
   LARGE_INTEGER m_current_count;
   LARGE_INTEGER m_previous_count;
   LARGE_INTEGER m_frequency_count;

   double m_delta_time = 0.0;		 //������ ������ �ð� ����
   double m_accumulate_time = 0.0;	 //1�� üũ�� ���� ���� �ð�

   UINT m_func_call_count = 0; //�Լ� ȣ�� ȸ�� üũ
   UINT m_fps = 0; //�ʴ� ȣ�� Ƚ��
};