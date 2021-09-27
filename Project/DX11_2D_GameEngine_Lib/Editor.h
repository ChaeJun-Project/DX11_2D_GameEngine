#pragma once

#include "Singleton.h"

//Editor의 상태를 정의
enum EditorState : UINT
{
    EditorState_Edit  = 0U,      //0000 편집 모드
    EditorState_Play  = 1U << 0, //0001 재생 모드
    EditorState_Pause = 1U << 1, //0010 정지 모드(재생 모드에서만 유효)
};

class Editor final : public Singleton<Editor>
{
   SINGLETON(Editor);

   Editor();
   ~Editor();

public:
   void Update();

public:
   const UINT& GetEngineState() { return this->editor_state; }
   void SetEngineState(const UINT& editor_state) { this->editor_state = editor_state; }

   void AddEngineState(const UINT& editor_state) { this->editor_state |= editor_state; }
   void DeleteEngineState(const UINT& editor_state) { this->editor_state &= ~editor_state; }

private:
    UINT editor_state = EditorState::EditorState_Edit;

};

