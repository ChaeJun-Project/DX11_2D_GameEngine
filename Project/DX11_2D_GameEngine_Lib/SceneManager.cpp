#include "stdafx.h"
#include "SceneManager.h"

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"

//GameLogic
#include "GameLogic_Script.h"

SceneManager::~SceneManager()
{
	m_p_current_scene.reset();
}

void SceneManager::Initialize()
{
	m_p_current_scene = std::make_shared<Scene>();

	auto game_logic = new GameObject();
	game_logic->SetObjectName("Game Logic");
	game_logic->SetObjectTag("Game Logic");
	game_logic->AddComponent(new Transform());
	game_logic->AddComponent(new GameLogic_Script());

	m_p_current_scene->AddGameObject(game_logic, 0, false);

	game_logic->GetComponent<Script>()->Initialize();
}

void SceneManager::Update()
{
	m_p_current_scene->Update();
	m_p_current_scene->LateUpdate();
	m_p_current_scene->FinalUpdate();

	//Update Collisio Manager
	CollisionManager::GetInstance()->Update();

	//Graphics Manager
	auto graphis_manager = GraphicsManager::GetInstance();

	//Graphics Render
	graphis_manager->BeginScene();
	m_p_current_scene->Render();
	graphis_manager->EndScene();

	//Event Manager
	EventManager::GetInstance()->Update();
}

void SceneManager::CreatePrefab(GameObject* p_game_object)
{
	p_game_object->RegisterPrefab();
}
