#pragma once

#include "Singleton.h"

//Editor�� ���¸� ����
enum EditorState : UINT
{
    EditorState_Edit  = 0U,      //0000 ���� ���
    EditorState_Play  = 1U << 0, //0001 ��� ���
    EditorState_Pause = 1U << 1, //0010 ���� ���(��� ��忡���� ��ȿ)
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

