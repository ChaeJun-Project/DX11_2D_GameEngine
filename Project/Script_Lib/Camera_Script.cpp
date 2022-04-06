#include "stdafx.h"
#include "Camera_Script.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/RenderManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>

Camera_Script::Camera_Script()
	:Script("Camera_Script")
{
	RegisterScriptParamData();
}

Camera_Script::Camera_Script(const Camera_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();
}

Camera_Script::~Camera_Script()
{
	m_p_transform = nullptr;
	m_p_camera = nullptr;

	m_p_target_transform = nullptr;
}

void Camera_Script::Awake()
{
	m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	m_p_camera = m_p_owner_game_object->GetComponent<Camera>();

	m_view_resolution = SETTINGS->GetGameResolution();
}

void Camera_Script::Update()
{
	if (m_p_transform == nullptr || m_p_camera == nullptr || m_p_target_transform == nullptr)
		return;

	auto target_position = m_p_target_transform->GetTranslation();
	m_p_transform->SetLocalTranslation(target_position);

	auto camera_position = m_p_transform->GetLocalTranslation();

	//카메라 영역이 맵 왼쪽을 벗어나는 경우
	if (-(m_map_resolution.x * 0.5f) > (camera_position.x - (m_view_resolution.x * 0.5f)))
	{
		camera_position.x = -(0.5f * (m_map_resolution.x - m_view_resolution.x));
	}
		
	//카메라 영역이 맵 오른쪽을 벗어나는 경우
	if ((m_map_resolution.x * 0.5f) < (camera_position.x + (m_view_resolution.x * 0.5f)))
	{
		camera_position.x = (0.5f * (m_map_resolution.x - m_view_resolution.x));
	}
	
	//카메라 영역이 맵 위쪽을 벗어나는 경우
	if ((m_map_resolution.y * 0.5f) < (camera_position.y + (m_view_resolution.y * 0.5f)))
	{
		camera_position.y = (0.5f * (m_map_resolution.y - m_view_resolution.y));
	}
	
	//카메라 영역이 맵 아래쪽을 벗어나는 경우
	if (-(m_map_resolution.y * 0.5f) > (camera_position.y - (m_view_resolution.y * 0.5f)))
	{
		camera_position.y = -(0.5f * (m_map_resolution.y - m_view_resolution.y));
	}

	m_p_transform->SetLocalTranslation(camera_position);
}

void Camera_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Map Resolution", ScriptParamType::Vector2, reinterpret_cast<void*>(&m_map_resolution), 100.f));
}

void Camera_Script::SetTarget(GameObject* p_player_game_object)
{
	m_p_target_transform = p_player_game_object->GetComponent<Transform>();

	auto target_position = m_p_target_transform->GetTranslation();
	m_p_transform->SetTranslation(target_position);
}

void Camera_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script

	//Map Resolution
	fprintf_s(p_file, "[Map Resolution]\n");
	FILE_MANAGER->FPrintf_Vector2(m_map_resolution, p_file);
}

void Camera_Script::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = { 0 };

	//Map Resolution
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector2(m_map_resolution, p_file);
}