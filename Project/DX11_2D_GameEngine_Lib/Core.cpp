#include "stdafx.h"
#include "Core.h"

#include "Settings.h"

#pragma region Manager
//FileManager
#include "FileManager.h"

//TimeManager
#include "TimeManager.h"

//InputManager
#include "InputManager.h"

//GraphicsManager
#include "GraphicsManager.h"

//Resource Manager
#include "ResourceManager.h"

//SceneManager
#include "SceneManager.h"

//RenderManager
#include "RenderManager.h"

//FontManager
#include "FontManager.h"

//AudioManager
#include "AudioManager.h"
#pragma endregion

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
	FileManager::Initialize();
    //Initialize Time Manager 
	TimeManager::GetInstance()->Initialize();
	//Initialize Input Manager
	InputManager::GetInstance()->Initialize();
	//Initialize Graphics Manager
	GraphicsManager::GetInstance()->Initialize();
	//Initialize Audio Manager
	AudioManager::GetInstance()->Initialize();
	//Initialize Resource Manager
	ResourceManager::GetInstance()->Initialize();
	//Initialize Scene Manager
	SceneManager::GetInstance()->Initialize();
	//Initialize Render Manager
	RenderManager::GetInstance()->Initialize();
	//Initialize Font Manager
	FontManager::GetInstance()->Initialize();
}

void Core::Progress()
{
	//Update Time Manager
	TimeManager::GetInstance()->Update();
	//Update Input Manager
	InputManager::GetInstance()->Update();
	//Update Scene Manager
	SceneManager::GetInstance()->Update();
	//Render Render Manager 
	RenderManager::GetInstance()->Render();
	//Update Audio Manager
	AudioManager::GetInstance()->Update();
	//Update Event Manager
	EventManager::GetInstance()->Update();
}
