#pragma once

#include "Singleton.h"

//Engine�� ���¸� ����
enum EngineState : UINT
{
    EngineState_Edit  = 0U,      //0000 ���� ���
    EngineState_Play  = 1U << 0, //0001 ��� ���
    EngineState_Pause = 1U << 1, //0010 ���� ���(��� ��忡���� ��ȿ)
};

class Engine final : public Singleton<Engine>
{
   SINGLETON(Engine);

   Engine();
   ~Engine();

public:
   void Update();

public:
   const UINT& GetEngineState() { return this->engine_state; }
   void SetEngineState(const UINT& engine_state) { this->engine_state = engine_state; }

   void AddEngineState(const UINT& engine_state) { this->engine_state |= engine_state; }
   void DeleteEngineState(const UINT& engine_state) { this->engine_state &= ~engine_state; }

private:
    UINT engine_state = EngineState::EngineState_Edit;

};

