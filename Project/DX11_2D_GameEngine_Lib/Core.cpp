#include "stdafx.h"
#include "Core.h"

#include "Settings.h"

Core::Core()
{
	//Create Settings
	m_p_settings = std::make_shared<Settings>();
}

Core::~Core()
{
	m_p_settings.reset();
}

void Core::Initialize()
{
    //Initialize File Manager
	FILE_MANAGER->Initialize();
    //Initialize Time Manager 
	TIME_MANAGER->Initialize();
	//Initialize Input Manager
	INPUT_MANAGER->Initialize();
	//Initialize Graphics Manager
	GRAPHICS_MANAGER->Initialize();
	//Initialize Audio Manager
	AUDIO_MANAGER->Initialize();
	//Initialize Scene Manager
	SCENE_MANAGER->Initialize();
	//Initialize Resource Manager
	RESOURCE_MANAGER->Initialize();
	//Initialize Render Manager
	RENDER_MANAGER->Initialize();
	//Initialize Font Manager
	FONT_MANAGER->Initialize();
}

void Core::Progress()
{
	//Update Time Manager
	TIME_MANAGER->Update();
	//Update Input Manager
	INPUT_MANAGER->Update();
	//Update Scene Manager
	SCENE_MANAGER->Update();
	//Render Render Manager 
	RENDER_MANAGER->Render();
	//Update Audio Manager
	AUDIO_MANAGER->Update();
	//Update Event Manager
	EVENT_MANAGER->Update();
}
