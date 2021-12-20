#include "stdafx.h"
#include "ClientSceneManager.h"

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

	auto scene_manager = SceneManager::GetInstance();
	auto resource_manager = ResourceManager::GetInstance();

	scene_manager->SetCurrentScene(new_scene);

	//Camera(0)
	auto camera = new GameObject();
	camera->SetGameObjectName("Main Camera");
	camera->SetGameObjectTag("Main Camera");
	camera->AddComponent(new Transform());
	camera->AddComponent(new Camera());

	camera->GetComponent<Camera>()->SetMainCamera();

	new_scene->AddGameObject(camera, 0, true);

	//Light2D
	auto point_light2D = new GameObject();
	point_light2D->SetGameObjectName("Light2D_Point");
	point_light2D->SetGameObjectTag("Light2D_Point");
	point_light2D->AddComponent(new Transform());
	point_light2D->AddComponent(new Light2D());

	auto point_light = point_light2D->GetComponent<Light2D>();
	point_light->SetLightType(LightType::Point);
	point_light->SetLightRange(2000.0f);
	point_light->SetLightColor(Color4::White);

	new_scene->AddGameObject(point_light2D, 0, true);

	//Game Logic
	auto game_logic = new GameObject();
	game_logic->SetGameObjectName("Game Logic");
	game_logic->SetGameObjectTag("Game Logic");
	game_logic->AddComponent(new Transform());
	game_logic->AddComponent(new GameLogic_Script());

	new_scene->AddGameObject(game_logic, 1, false);

	game_logic->GetComponent<Script>()->Initialize();

	//Tile Object
	auto game_object = new GameObject();
	game_object->SetGameObjectName("TileObject");
	game_object->SetGameObjectTag("Tile");
	game_object->AddComponent(new Transform());
	game_object->AddComponent(new TileMap());

	auto tile_map = game_object->GetComponent<TileMap>();

	new_scene->AddGameObject(game_object, 3, true);
}

void ClientSceneManager::SaveScene(const std::string& file_path)
{
	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();

	FILE* p_file = nullptr;
	fopen_s(&p_file, file_path.c_str(), "wb");
	assert(p_file);

	current_scene->SaveToScene(p_file);

	const auto& layer_map = current_scene->GetLayerMap();

	for (auto& layer : layer_map)
	{
		layer.second->SaveToScene(p_file);

		fprintf(p_file, "[Parent GameObject Count]\n");
		const auto& parent_game_object_vector = layer.second->GetParentGameObjects();
		auto parent_game_object_count = parent_game_object_vector.size();
		fprintf(p_file, "%d\n", parent_game_object_count);
		//fwrite(&parent_game_object_count, sizeof(size_t), 1, p_file); //해당 Layer의 부모 GameObject 개수 쓰기

		for (auto& parent_game_object : parent_game_object_vector)
		{
			//SaveGameObject(parent_game_object, p_file);
		}

		fprintf(p_file, "\n");
	}

	fclose(p_file);
}

std::shared_ptr<Scene> ClientSceneManager::LoadScene(const std::string& file_path)
{
	auto new_scene = std::make_shared<Scene>("New Scene");

	FILE* p_file = nullptr;
	fopen_s(&p_file, file_path.c_str(), "rb");
	assert(p_file);

	new_scene->LoadFromScene(p_file);

	const auto& layer_map = new_scene->GetLayerMap();

	for (auto& layer : layer_map)
	{
		layer.second->LoadFromScene(p_file);

		char char_buffer[256] = {};
		FileManager::FScanf(char_buffer, p_file);
		int parent_game_object_count = 0;
		fscanf_s(p_file, "%d\n", &parent_game_object_count);//해당 Layer의 부모 GameObject 개수 읽어오기

		for (UINT i = 0; i < static_cast<UINT>(parent_game_object_count); ++i)
		{
			GameObject* new_game_object = LoadGameObject(p_file);
			layer.second->AddGameObject(new_game_object, false);
		}
	}

	fclose(p_file);

	return new_scene;
}

void ClientSceneManager::SaveGameObject(GameObject* p_game_object, FILE* p_file)
{
	p_game_object->SaveToScene(p_file);

	for (UINT i = 1; i < static_cast<UINT>(ComponentType::END); ++i)
	{
		IComponent* p_component = p_game_object->GetComponent(static_cast<ComponentType>(i));

		if(p_component != nullptr)
			p_component->SaveToScene(p_file);
	}
}

GameObject* ClientSceneManager::LoadGameObject(FILE* p_file)
{
	return nullptr;
}

void ClientSceneManager::CreatePrefab()
{
}
