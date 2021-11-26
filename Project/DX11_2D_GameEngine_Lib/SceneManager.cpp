#include "stdafx.h"
#include "SceneManager.h"

#include "RenderManager.h"

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "Camera.h"

//GameLogic
#include "GameLogic_Script.h"

#include "ParticleSystem.h"

SceneManager::~SceneManager()
{
	m_p_current_scene.reset();
}

void SceneManager::Initialize()
{
	m_p_current_scene = std::make_shared<Scene>("New Scene");

	auto resource_manager = ResourceManager::GetInstance();

	//Camera(0)
	auto camera = new GameObject();
	camera->SetObjectName("Main Camera");
	camera->SetObjectTag("Main Camera");
	camera->AddComponent(new Transform());
	camera->AddComponent(new Camera());

	camera->GetComponent<Camera>()->SetMainCamera();

	m_p_current_scene->AddGameObject(camera, 0, true);

	//Game Logic
	auto game_logic = new GameObject();
	game_logic->SetObjectName("Game Logic");
	game_logic->SetObjectTag("Game Logic");
	game_logic->AddComponent(new Transform());
	game_logic->AddComponent(new GameLogic_Script());

	m_p_current_scene->AddGameObject(game_logic, 1, false);

	game_logic->GetComponent<Script>()->Initialize();
}

void SceneManager::Update()
{
	if (m_editor_state == EditorState::EditorState_Play || m_client_state == 1)
	{
		m_p_current_scene->Update();
		m_p_current_scene->LateUpdate();
	}

	m_p_current_scene->FinalUpdate();

	if (m_editor_state == EditorState::EditorState_Play || m_client_state == 1)
		//Update Collisio Manager
		CollisionManager::GetInstance()->Update();
}

void SceneManager::CreatePrefab(GameObject* p_game_object)
{
	p_game_object->RegisterPrefab();
}
