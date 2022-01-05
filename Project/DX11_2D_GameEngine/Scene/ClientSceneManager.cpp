#include "stdafx.h"
#include "ClientSceneManager.h"

#include "Helper/EditorHelper.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>
#include <DX11_2D_GameEngine_Lib/Layer.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/IComponent.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>
#include <DX11_2D_GameEngine_Lib/Light2D.h>
#include <DX11_2D_GameEngine_Lib/TileMap.h>

#include <Script_Lib/GameLogic_Script.h>

void ClientSceneManager::CreateNewScene()
{
	auto new_scene = std::make_shared<Scene>("New Scene");
	new_scene->Initialize();

	auto scene_manager = SceneManager::GetInstance();
	auto resource_manager = ResourceManager::GetInstance();

	scene_manager->SetCurrentScene(new_scene);

	return;

	//Camera(0)
	auto camera = new GameObject();
	camera->SetGameObjectName("Main Camera");
	camera->SetGameObjectTag("Main Camera");
	camera->AddComponent(new Transform());
	camera->AddComponent(new Camera());

	camera->GetComponent<Camera>()->SetMainCamera();

	new_scene->RegisterGameObject(camera);

	//Light2D
	auto point_light2D = new GameObject();
	point_light2D->SetGameObjectName("Light2D_Point");
	point_light2D->SetGameObjectTag("Light2D_Point");
	point_light2D->AddComponent(new Transform());
	point_light2D->AddComponent(new Light2D());

	auto point_light = point_light2D->GetComponent<Light2D>();
	point_light->SetLightType(LightType::Point);
	point_light->SetLightRange(2000.0f);
	point_light->SetLightColor(Vector4::White);

	new_scene->RegisterGameObject(point_light2D);

	//Game Logic
	auto game_logic = new GameObject();
	game_logic->SetGameObjectName("Game Logic");
	game_logic->SetGameObjectTag("Game Logic");
	game_logic->AddComponent(new Transform());
	game_logic->AddComponent(new GameLogic_Script());

	new_scene->RegisterGameObject(game_logic);

	//Tile Object
	auto game_object = new GameObject();
	game_object->SetGameObjectName("TileObject");
	game_object->SetGameObjectTag("Tile");
	game_object->AddComponent(new Transform());
	game_object->AddComponent(new TileMap());

	auto tile_map = game_object->GetComponent<TileMap>();

	new_scene->RegisterGameObject(game_object);
}

std::shared_ptr<Scene> ClientSceneManager::SaveScene(const std::string& file_path)
{
	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();

	FILE* p_file = nullptr;
	fopen_s(&p_file, file_path.c_str(), "wb");
	assert(p_file);

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

	return current_scene;
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
	for (UINT i = 1; i < static_cast<UINT>(ComponentType::END); ++i)
	{
		IComponent* p_component = p_game_object->GetComponent(static_cast<ComponentType>(i));

		if (p_component != nullptr)
			p_component->SaveToScene(p_file);
	}

	//Script
	//SaveScript(p_game_object, p_file);

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
	fprintf(p_file, "[Script]\n");
	//const auto& script_vector;
}

std::shared_ptr<Scene> ClientSceneManager::LoadScene(const std::string& file_path)
{
	EditorHelper::GetInstance()->SetSelectedGameObject(nullptr);
	EditorHelper::GetInstance()->SetSelectedResource(nullptr);

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
	FileManager::FScanf(char_buffer, p_file);
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

	FileManager::FScanf(char_buffer, p_file);

	//==================================
	//GameObject
	//==================================
	FileManager::FScanf(char_buffer, p_file);
	//Create GameObject
	GameObject* p_new_game_object = new GameObject();
	p_new_game_object->LoadFromScene(p_file);

	//==================================
	//Component
	//==================================
	//Component Count
	FileManager::FScanf(char_buffer, p_file);
	UINT component_count = 0;
	fscanf_s(p_file, "%d\n", &component_count);

	//Create Component
	for (UINT i = 0; i < component_count; ++i)
	{
		//Component Title
		FileManager::FScanf(char_buffer, p_file);

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
	//LoadScript(p_new_game_object, p_file);

	//==================================
	//Child GameObject
	//==================================
	//Child GameObject Count
	FileManager::FScanf(char_buffer, p_file);
	UINT child_game_object_count = 0;
	fscanf_s(p_file, "%d\n", &child_game_object_count);

	//Create Child GameObject
	if (child_game_object_count != 0)
	{
		FileManager::FScanf(char_buffer, p_file);
		for (UINT i = 0; i < child_game_object_count; ++i)
		{
			GameObject* p_new_child_game_object = LoadGameObject(p_file);
			p_new_game_object->AddChild(p_new_child_game_object);
		}
	}

	FileManager::FScanf(char_buffer, p_file);

	return p_new_game_object;
}

void ClientSceneManager::LoadScript(GameObject* p_game_object, FILE* p_file)
{
}

void ClientSceneManager::CreatePrefab()
{
}
