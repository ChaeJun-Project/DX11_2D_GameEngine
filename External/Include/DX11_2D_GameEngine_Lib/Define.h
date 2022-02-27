#pragma once

//싱글톤 매크로 정의
//Singleton를 상속받는 type 클래스의 모든 멤버에 접근하여 사용하기 위해 friend 사용
#define SINGLETON(type) private:\
						friend class Singleton<type>;\

//메모리 해제 관련 매크로 정의
#define SAFE_DELETE(p)			{ if(p) { delete p; p = nullptr; } }
#define SATE_DELETE_ARRAY(p)	{ if(p) { delete[] p; p = nullptr;} }

//자원 정보 받기
#define SAFE_GET_POINTER(p)       { if(p) {return p;} return nullptr; }

//자원 메모리 해제 매크로 정의
#define SAFE_RELEASE(p)         { if(p) { p->Release(); p = nullptr; } }

//Manager
#define AUDIO_MANAGER		AudioManager::GetInstance()
#define COLLISION_MANAGER   CollisionManager::GetInstance()
#define EVENT_MANAGER		EventManager::GetInstance()
#define FILE_MANAGER		FileManager::GetInstance()
#define FONT_MANAGER		FontManager::GetInstance()
#define GRAPHICS_MANAGER	GraphicsManager::GetInstance()
#define INPUT_MANAGER		InputManager::GetInstance()
#define LOG_MANAGER			LogManager::GetInstance()
#define RENDER_MANAGER		RenderManager::GetInstance()
#define RESOURCE_MANAGER	ResourceManager::GetInstance()
#define SCENE_MANAGER		SceneManager::GetInstance()
#define TIME_MANAGER		TimeManager::GetInstance()

//Log
//Editor
#define EDITOR_LOG_INFO_F(text, ...)       { if(SCENE_MANAGER->GetClientState() == 2) {LOG_MANAGER->SetCallerName(__FUNCTION__); LOG_MANAGER->Info_Formatted(text, __VA_ARGS__); } }
#define EDITOR_LOG_WARNING_F(text, ...)    { if(SCENE_MANAGER->GetClientState() == 2) {LOG_MANAGER->SetCallerName(__FUNCTION__); LOG_MANAGER->Warning_Formatted(text, __VA_ARGS__); } }
#define EDITOR_LOG_ERROR_F(text, ...)      { if(SCENE_MANAGER->GetClientState() == 2) {LOG_MANAGER->SetCallerName(__FUNCTION__); LOG_MANAGER->Error_Formatted(text, __VA_ARGS__); } }
//Play
#define PLAY_LOG_INFO_F(text, ...)       { if(SCENE_MANAGER->GetClientState() == 1) {LOG_MANAGER->SetCallerName(__FUNCTION__); LOG_MANAGER->Info_Formatted(text, __VA_ARGS__); }
#define PLAY_LOG_WARNING_F(text, ...)    { if(SCENE_MANAGER->GetClientState() == 1) {LOG_MANAGER->SetCallerName(__FUNCTION__); LOG_MANAGER->Warning_Formatted(text, __VA_ARGS__); }
#define PLAY_LOG_ERROR_F(text, ...)      { if(SCENE_MANAGER->GetClientState() == 1) {LOG_MANAGER->SetCallerName(__FUNCTION__); LOG_MANAGER->Error_Formatted(text, __VA_ARGS__); }

//Time
//Delta Time
#define DELTA_TIME_D	TIME_MANAGER->GetDeltaTime_double()
#define DELTA_TIME_F	TIME_MANAGER->GetDeltaTime_float()
#define CURRENT_TIME	TIME_MANAGER->GetCurrentTime_str()

//Input
//Key 상태 체크 매크로 정의 
#define KEY_PRESS(key)  INPUT_MANAGER->KeyPress(key)
#define KEY_DOWN(key)	INPUT_MANAGER->KeyDown(key)
#define KEY_UP(key)		INPUT_MANAGER->KeyUp(key)

//Mouse 상태 체크 매크로 정의
#define MOUSE_BUTTON_DOWN(button)	 INPUT_MANAGER->BtnDown(button)
#define MOUSE_BUTTON_UP(button)		 INPUT_MANAGER->BtnUp(button)
#define MOUSE_BUTTON_PRESS(button)   INPUT_MANAGER->BtnPress(button)

#define MOUSE_MOVE		INPUT_MANAGER->GetMouseMoveValue()

//DirectX11 
//Device & Device Context
#define DEVICE			GRAPHICS_MANAGER->GetDevice()
#define DEVICE_CONTEXT	GRAPHICS_MANAGER->GetDeviceContext()

//Audio
#define AUDIO_SYSTEM	AUDIO_MANAGER->GetAudioSystem()

//클래스 복사 생성 매크로 정의(깊은 복사, 기존 클래스와 클론 클래스가 같은 메모리를 참조하지 않음)
#define CLONE(type) type* Clone() const {return new type(*this);}

//최대 Layer 개수
#define MAX_LAYER 32

//Path
#define ABSOLUTE_CONTENT_PATH  FILE_MANAGER->GetAbsoluteContentPath()
#define ANIMATION_PATH		   FILE_MANAGER->GetAbsoluteAnimationPath()
#define AUDIO_PATH			   FILE_MANAGER->GetAbsoluteAudioPath()
#define MATERIAL_PATH		   FILE_MANAGER->GetAbsoluteMaterialPath()
#define MESH_PATH			   FILE_MANAGER->GetAbsoluteMeshPath()
#define SCENE_PATH			   FILE_MANAGER->GetAbsoluteScenePath()
#define TEXTURE_PATH		   FILE_MANAGER->GetAbsoluteTexturePath()
#define TILEMAP_PATH		   FILE_MANAGER->GetAbsoluteTileMapPath()
#define PREFAB_PATH			   FILE_MANAGER->GetAbsolutePrefabPath()
