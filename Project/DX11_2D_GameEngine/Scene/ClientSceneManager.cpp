#include "stdafx.h"
#include "ClientSceneManager.h"

#include "Helper/EditorHelper.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>
#include <DX11_2D_GameEngine_Lib/Prefab.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
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
	Prefab::p_save_game_object_func = std::bind
	(
		&ClientSceneManager::SaveGameObject,
		std::placeholders::_1,
		std::placeholders::_2
	);

	Prefab::p_load_game_object_func = std::bind
	(
		&ClientSceneManager::LoadGameObject,
		std::placeholders::_1
	);
}

void ClientSceneManager::InitializeCurrentScene()
{
	EDITOR_HELPER->SetSelectedGameObject(nullptr);
	EDITOR_HELPER->SetSelectedResource(nullptr);

	//Current Scene
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	std::string absolute_scene_path = SCENE_PATH;
	absolute_scene_path += current_scene->GetSceneName();
	absolute_scene_path += ".scene";

	//현재 Scene을 다시 Load하여 초기상태로 변경
	auto init_scene = LoadScene(absolute_scene_path);

	//Scene Change
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Scene_Change;
	event_struct.object_address_1 = init_scene;

	EVENT_MANAGER->AddEvent(event_struct);
}

void ClientSceneManager::CreateNewScene()
{
	EDITOR_HELPER->SetSelectedGameObject(nullptr);
	EDITOR_HELPER->SetSelectedResource(nullptr);

	//Current Scene
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	//New Scene
	auto new_scene = std::make_shared<Scene>("New Scene");
	new_scene->SetStartScene();

	//Current Scene이 존재하지 않은 경우 => 최초 Scene 생성
	if (current_scene == nullptr)
		SCENE_MANAGER->SetCurrentScene(new_scene);

	//Current Scene이 존재하는 경우 => Scene 변경
	else
	{
		//Scene Change
		EventStruct event_struct;
		ZeroMemory(&event_struct, sizeof(EventStruct));

		event_struct.event_type = EventType::Scene_Change;
		event_struct.object_address_1 = new_scene;

		EVENT_MANAGER->AddEvent(event_struct);

		EDITOR_LOG_INFO_F("새로운 Scene을 성공적으로 만들었습니다.");
	}
}

bool ClientSceneManager::SaveScene(const std::string& file_path)
{
	EDITOR_HELPER->SetSelectedGameObject(nullptr);
	EDITOR_HELPER->SetSelectedResource(nullptr);

	auto scene_name = FILE_MANAGER->GetOriginFileNameFromPath(file_path);
	auto current_scene = SCENE_MANAGER->GetCurrentScene();
	if (!current_scene->GetSceneName()._Equal(scene_name))
	{
		current_scene->SetSceneName(scene_name);
		UpdateScene();
	}

	FILE* p_file = nullptr;
	fopen_s(&p_file, file_path.c_str(), "wb");

	if (p_file != nullptr)
	{

		//==================================
		//Scene
		//==================================
		current_scene->SaveToScene(p_file);

		//==================================
		//Parent GameObjects
		//==================================
		fprintf(p_file, "[Parent GameObject Count]\n");
		const auto& parent_game_object_vector = current_scene->GetAllParentGameObjects();
		auto parent_game_object_count = parent_game_object_vector.size();
		fprintf(p_file, "%d\n", parent_game_object_count); //해당 Scene에 속한 Parent GameObject 개수

		for (auto& parent_game_object : parent_game_object_vector)
		{
			SaveGameObject(parent_game_object, p_file);
		}

		fprintf(p_file, "\n");

		fclose(p_file);

		return true;
	}

	else
	   return false;
}

void ClientSceneManager::SaveGameObject(GameObject* p_game_object, FILE* p_file)
{
	fprintf(p_file, "=================================================================================\n");

	//==================================
	//GameObject
	//==================================
	fprintf(p_file, "◆ GameObject\n");
	p_game_object->SaveToScene(p_file);

	//==================================
	//Component
	//==================================
	//Component Count
	fprintf(p_file, "[Component Count]\n");
	auto component_count = p_game_object->GetComponentCount();
	fprintf(p_file, "%d\n", component_count);

	//Save Component
	for (UINT i = static_cast<UINT>(ComponentType::None); i < static_cast<UINT>(ComponentType::Script); ++i)
	{
		IComponent* p_component = p_game_object->GetComponent(static_cast<ComponentType>(i));

		if (p_component != nullptr)
			p_component->SaveToScene(p_file);
	}

	//Save Script
	SaveScript(p_game_object, p_file);

	//==================================
	//Child GameObject
	//==================================
	//Child GameObject Count
	fprintf(p_file, "[Child GameObject Count]\n");
	const auto& child_game_object_vector = p_game_object->GetChilds();
	auto child_game_object_count = child_game_object_vector.size();
	fprintf(p_file, "%d\n", child_game_object_count);

	//Save Child GameObject
	if (p_game_object->HasChilds())
	{
		fprintf(p_file, "[Child GameObject]\n");
		for (auto& child_game_object : child_game_object_vector)
		{
			SaveGameObject(child_game_object, p_file);
		}
	}

	fprintf(p_file, "=================================================================================\n");
}

void ClientSceneManager::SaveScript(GameObject* p_game_object, FILE* p_file)
{
	const auto& script_un_map = p_game_object->GetScriptUnMap();

	fprintf(p_file, "[Script Count]\n");
	auto script_count = script_un_map.size();
	fprintf(p_file, "%d\n", script_count);

	for (const auto& script : script_un_map)
	{
		script.second->SaveToScene(p_file);
	}
}

std::shared_ptr<Scene> ClientSceneManager::LoadScene(const std::string& file_path)
{
	EDITOR_HELPER->SetSelectedGameObject(nullptr);
	EDITOR_HELPER->SetSelectedResource(nullptr);

	auto p_new_scene = std::make_shared<Scene>("New Scene");

	FILE* p_file = nullptr;
	fopen_s(&p_file, file_path.c_str(), "rb");
	assert(p_file);

	//==================================
	//Scene
	//==================================
	p_new_scene->LoadFromScene(p_file);

	//==================================
	//Parent GameObjects
	//==================================
	char char_buffer[256] = {};
	FILE_MANAGER->FScanf(char_buffer, p_file);
	UINT parent_game_object_count = 0;
	fscanf_s(p_file, "%d\n", &parent_game_object_count); //해당 Scene에 속한 Parent GameObject 개수 읽어오기

	for (UINT i = 0; i < parent_game_object_count; ++i)
	{
		GameObject* p_parent_game_object = LoadGameObject(p_file);
		p_new_scene->RegisterGameObject(p_parent_game_object); //해당 Scene에 Load한 GameObject를 등록
	}

	fclose(p_file);

	return p_new_scene;
}


GameObject* ClientSceneManager::LoadGameObject(FILE* p_file)
{
	char char_buffer[256] = {};

	FILE_MANAGER->FScanf(char_buffer, p_file);

	//==================================
	//GameObject
	//==================================
	FILE_MANAGER->FScanf(char_buffer, p_file);
	//Create GameObject
	GameObject* p_new_game_object = new GameObject();
	p_new_game_object->LoadFromScene(p_file);

	//==================================
	//Component
	//==================================
	//Component Count
	FILE_MANAGER->FScanf(char_buffer, p_file);
	UINT component_count = 0;
	fscanf_s(p_file, "%d\n", &component_count);

	//Create Component
	for (UINT i = 0; i < component_count; ++i)
	{
		//Component Title
		FILE_MANAGER->FScanf(char_buffer, p_file);

		//Component Type
		UINT component_type = 0;
		fscanf_s(p_file, "%d\n", &component_type);

		p_new_game_object->AddComponent(static_cast<ComponentType>(component_type));

		auto p_component = p_new_game_object->GetComponent(static_cast<ComponentType>(component_type));

		if (p_component != nullptr)
			p_component->LoadFromScene(p_file);
	}

	//==================================
	//Script
	//==================================
	LoadScript(p_new_game_object, p_file);

	//==================================
	//Child GameObject
	//==================================
	//Child GameObject Count
	FILE_MANAGER->FScanf(char_buffer, p_file);
	UINT child_game_object_count = 0;
	fscanf_s(p_file, "%d\n", &child_game_object_count);

	//Create Child GameObject
	if (child_game_object_count != 0)
	{
		FILE_MANAGER->FScanf(char_buffer, p_file);
		for (UINT i = 0; i < child_game_object_count; ++i)
		{
			GameObject* p_new_child_game_object = LoadGameObject(p_file);
			p_new_game_object->AddChild(p_new_child_game_object);
		}
	}

	FILE_MANAGER->FScanf(char_buffer, p_file);

	return p_new_game_object;
}

void ClientSceneManager::LoadScript(GameObject* p_game_object, FILE* p_file)
{
	char char_buffer[256] = {};

	//Script Count
	FILE_MANAGER->FScanf(char_buffer, p_file);
	UINT script_count = 0;
	fscanf_s(p_file, "%d\n", &script_count);

	//Create Script
	for (UINT i = 0; i < script_count; ++i)
	{
		//Component Title
		FILE_MANAGER->FScanf(char_buffer, p_file);

		//Component Type
		UINT component_type = 0;
		fscanf_s(p_file, "%d\n", &component_type);

		//Script Active
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Active]
		int boolen_num = -1;
		fscanf_s(p_file, "%d\n", &boolen_num);

		//Script Name
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Name]
		FILE_MANAGER->FScanf(char_buffer, p_file); //Script Name
		std::string script_name = char_buffer;

		//Add Script
		p_game_object->AddComponent(ScriptManager::GetScript(script_name));

		auto p_script = p_game_object->GetScript(script_name);
		p_script->SetIsActive(boolen_num); //Set Is Active

		if (p_script != nullptr)
			p_script->LoadFromScene(p_file);
	}
}