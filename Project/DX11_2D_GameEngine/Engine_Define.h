#pragma once

typedef std::function<void(void)> Event_CallBack;
typedef std::function<void(std::string)> Clicked_CallBack1;
typedef std::function<void(DWORD_PTR)> Clicked_CallBack2;
typedef std::function<void(DWORD_PTR, DWORD_PTR)> DragDrop_CallBack;

#define CAN_EDIT SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop