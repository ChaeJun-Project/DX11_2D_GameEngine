#include "stdafx.h"
#include "ClientSceneManager.h"

#include "Helper/EditorHelper.h"

#include <DX11_2D_GameEngine_Lib/Scene.h>
#include <DX11_2D_GameEngine_Lib/Layer.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/IComponent.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>
#include <DX11_2D_GameEngine_Lib/Light2D.h>
#include <DX11_2D_GameEngine_Lib/TileMapRenderer.h>
#include <DX11_2D_GameEngine_Lib/Script.h>

//Script
#include <Script_Lib/ScriptManager.h>

void ClientSceneManager::Initialize()
{
	SCENE_MANAGER->SetGetScriptFunc(
	std::bind
	(
		&ScriptManager::GetScript,
		std::placeholders::_1
	)
	);
}

void ClientSceneManager::InitializeScene()
{
	EDITOR_HELPER->SetSelectedGameObject(nullptr);
	EDITOR_HELPER->SetSelectedResource(nullptr);

	SCENE_MANAGER->InitializeScene(m_recent_scene_name);
}

void ClientSceneManager::CreateNewScene()
{
	EDITOR_HELPER->SetSelectedGameObject(nullptr);
	EDITOR_HELPER->SetSelectedResource(nullptr);

	SCENE_MANAGER->CreateNewScene();
}

bool ClientSceneManager::SaveScene(const std::string& file_path)
{
	return SCENE_MANAGER->SaveScene(file_path);
}

std::shared_ptr<Scene> ClientSceneManager::LoadScene(const std::string& file_path)
{
	EDITOR_HELPER->SetSelectedGameObject(nullptr);
	EDITOR_HELPER->SetSelectedResource(nullptr);

	return SCENE_MANAGER->LoadScene(file_path);
}