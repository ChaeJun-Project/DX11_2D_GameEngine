#include "stdafx.h"
#include "Camera_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>

Camera_Script::Camera_Script()
	:Script("Camera_Script")
{
}

Camera_Script::Camera_Script(const Camera_Script& origin)
	: Script(origin.m_script_name)
{
}

Camera_Script::~Camera_Script()
{
}

void Camera_Script::Initialize()
{
}

void Camera_Script::Start()
{
}

void Camera_Script::Update()
{
}

void Camera_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}

void Camera_Script::LoadFromScene(FILE* p_file)
{
}