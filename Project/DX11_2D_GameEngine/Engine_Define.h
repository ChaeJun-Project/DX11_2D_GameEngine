#pragma once

typedef std::function<void(void)> Event_CallBack;
typedef std::function<void(std::string)> Clicked_CallBack1;
typedef std::function<void(DWORD_PTR)> Clicked_CallBack2;
typedef std::function<void(DWORD_PTR, DWORD_PTR)> DragDrop_CallBack;

#define CAN_EDIT SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Stop

#define EDITOR_MANAGER EditorManager::GetInstance()
#define EDITOR_HELPER EditorHelper::GetInstance()
#define EDITOR_OBJECT_MANAGER EditorObjectManager::GetInstance()
#define ICON_PROVIDER IconProvider::GetInstance()