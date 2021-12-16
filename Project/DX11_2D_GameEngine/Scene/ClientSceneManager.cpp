#include "stdafx.h"
#include "ClientSceneManager.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>
#include <DX11_2D_GameEngine_Lib/Layer.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>
#include <DX11_2D_GameEngine_Lib/Light2D.h>

void ClientSceneManager::CreateNewScene()
{
	auto new_scene = std::make_shared<Scene>("New Scene");

	auto scene_manager = SceneManager::GetInstance();
	auto resource_manager = ResourceManager::GetInstance();

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
	/*auto game_logic = new GameObject();
	game_logic->SetGameObjectName("Game Logic");
	game_logic->SetGameObjectTag("Game Logic");
	game_logic->AddComponent(new Transform());
	game_logic->AddComponent(new GameLogic_Script());

	new_scene->AddGameObject(game_logic, 1, false);

	game_logic->GetComponent<Script>()->Initialize();*/

	//Tile Object
	/*auto game_object = new GameObject();
	game_object->SetGameObjectName("TileObject");
	game_object->SetGameObjectTag("Tile");
	game_object->AddComponent(new Transform());
	game_object->AddComponent(new TileMap());

	auto tile_map = game_object->GetComponent<TileMap>();

	new_scene->AddGameObject(game_object, 3, true);*/

	scene_manager->SetCurrentScene(new_scene);
}

void ClientSceneManager::SaveScene(const std::string& file_path)
{
}

Scene* ClientSceneManager::LoadScene(const std::string& file_path)
{
	return nullptr;
}

void ClientSceneManager::SaveGameObject(GameObject* p_game_object, FILE* p_file)
{
}

GameObject* ClientSceneManager::LoadGameObject(FILE* p_file)
{
	return nullptr;
}

void ClientSceneManager::CreatePrefab()
{
}
