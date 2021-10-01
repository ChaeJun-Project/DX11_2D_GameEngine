#pragma once

#include "Singleton.h"

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

