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

std::string ClientSceneManager::m_recent_scene_name;

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

void ClientSceneManager::InitializeRecentScene()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_recent_scene_file_path);

	if (FILE_MANAGER->IsExistFile(path))
		LoadRecentScene();

	else
		SaveRecentScene();

	if (!m_recent_scene_name.empty())
	{
		auto recent_scene_path = FILE_MANAGER->GetAbsoluteContentPath();
		recent_scene_path += ("Asset/Scene/" + m_recent_scene_name + ".scene");

		FileFunction::LoadScene(recent_scene_path);
	}

	else
	{
		CreateNewScene();
	}
}

void ClientSceneManager::LoadRecentScene()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_recent_scene_file_path);

	FILE* p_file = nullptr;

	fopen_s(&p_file, path.c_str(), "rb");
	if (p_file != nullptr)
	{
		char char_buffer[256] = { 0 };

		//Recent Scene
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Recent Scene]
		FILE_MANAGER->FScanf(char_buffer, p_file); //Recent Scene Name
		m_recent_scene_name = std::string(char_buffer);

		fclose(p_file);
	}
}

void ClientSceneManager::SaveRecentScene()
{
	std::string path = FILE_MANAGER->GetAbsoluteContentPath();
	path += (m_folder_path + m_recent_scene_file_path);

	FILE* p_file = nullptr;
	fopen_s(&p_file, path.c_str(), "wb");

	if (p_file != nullptr)
	{
		//Recent Scene
		fprintf(p_file, "[Recent Scene]\n");
		fprintf(p_file, "%s\n", m_recent_scene_name.c_str());

		fclose(p_file);
	}
}
