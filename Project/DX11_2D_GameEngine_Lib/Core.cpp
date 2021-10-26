#include "stdafx.h"
#include "Core.h"

#include "Settings.h"

#pragma region Manager
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
#pragma endregion

Core::Core()
{
	//Create Settings
	this->m_p_settings = std::make_shared<Settings>();
}

Core::~Core()
{
	this->m_p_settings.reset();
}

void Core::Initialize()
{
    //Initialize Time Manager 
	TimeManager::GetInstance()->Initialize();
	//Initialize Input Manager
	InputManager::GetInstance()->Initialize();
	//Initialize Graphics Manager
	GraphicsManager::GetInstance()->Initialize();
	//Initialize Resource Manager
	ResourceManager::GetInstance()->Initialize();
	//Initialize Scene Manager
	SceneManager::GetInstance()->Initialize();
}

void Core::Progress()
{
	//Update Time Manager
	TimeManager::GetInstance()->Update();
	//Update Input Manager
	InputManager::GetInstance()->Update();
	//Update Scene Manager
	SceneManager::GetInstance()->Update();
}
