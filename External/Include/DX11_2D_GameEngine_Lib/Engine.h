#pragma once

#include "Singleton.h"

//Engine의 상태를 정의
enum EngineState : UINT
{
    EngineState_Edit  = 0U,      //0000 편집 모드
    EngineState_Play  = 1U << 0, //0001 재생 모드
    EngineState_Pause = 1U << 1, //0010 정지 모드(재생 모드에서만 유효)
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

