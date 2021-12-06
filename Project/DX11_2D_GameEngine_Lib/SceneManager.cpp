#include "stdafx.h"
#include "SceneManager.h"

#include "RenderManager.h"

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "Camera.h"
#include "Animator2D.h"
#include "ParticleSystem.h"
#include "TileMap.h"

//GameLogic
#include "GameLogic_Script.h"

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

	//Light2D
	auto point_light2D = new GameObject();
	point_light2D->SetObjectName("Light2D_Point");
	point_light2D->SetObjectTag("Light2D_Point");
	point_light2D->AddComponent(new Transform());
	point_light2D->AddComponent(new Light2D());

	auto point_light = point_light2D->GetComponent<Light2D>();
	point_light->SetLightType(LightType::Point);
	point_light->SetLightRange(2000.0f);
	point_light->SetLightColor(Color4::White);

	m_p_current_scene->AddGameObject(point_light2D, 0, true);

	//Game Logic
	/*auto game_logic = new GameObject();
	game_logic->SetObjectName("Game Logic");
	game_logic->SetObjectTag("Game Logic");
	game_logic->AddComponent(new Transform());
	game_logic->AddComponent(new GameLogic_Script());

	m_p_current_scene->AddGameObject(game_logic, 1, false);

	game_logic->GetComponent<Script>()->Initialize();*/

	//Tile Object
	auto game_object = new GameObject();
	game_object->SetObjectName("TileObject");
	game_object->SetObjectTag("Tile");
	game_object->AddComponent(new Transform());
	game_object->AddComponent(new TileMap());

	auto tile_map = game_object->GetComponent<TileMap>();

	//tile_map->SetTileAtlasTexture(ResourceManager::GetInstance()->GetTileAtlasTexture("Geometry_Tile"));

	m_p_current_scene->AddGameObject(game_object, 3, true);
}

void SceneManager::Update()
{
	//<summary>
	//ClientState
	//Title = 0
	//Game = 1
	//Editor = 2
	//</summary>
	
	//Editor State 비트값에 Play가 포함되어있고 Pause가 비포함되어있는 경우 또는 게임모드인 경우
	if ( ((m_editor_state & EditorState::EditorState_Play) && !(m_editor_state & EditorState::EditorState_Pause))
	|| m_client_state == 1 )
	{
		m_p_current_scene->Update();
		m_p_current_scene->LateUpdate();
	}

	m_p_current_scene->FinalUpdate();

	//Editor State 비트값에 Play가 포함되어있고 Pause가 비포함되어있는 경우 또는 게임모드인 경우
	if (((m_editor_state & EditorState::EditorState_Play) && !(m_editor_state & EditorState::EditorState_Pause))
		|| m_client_state == 1)
		//Update Collisio Manager
		CollisionManager::GetInstance()->Update();
}

void SceneManager::CreatePrefab(GameObject* p_game_object)
{
	p_game_object->RegisterPrefab();
}

void SceneManager::SetEditorState(const UINT& editor_state)
{
	//정지상태로 변경한다면
	if (editor_state == EditorState::EditorState_Stop)
	{
		m_editor_state = editor_state;
	}

	else
	{
		//이미 현재 적용된 상태라면
		if (m_editor_state & editor_state)
		{
			m_editor_state &= ~editor_state; //해당 상태 제거
		}

		//현재 적용된 상태가 아니라면 추가
		else
		{
			if (m_editor_state == EditorState::EditorState_Stop &&
				editor_state == EditorState::EditorState_Play)
			{
				m_p_current_scene->Start();
			}

			m_editor_state |= editor_state;
		}
	}
}