#include "stdafx.h"
#include "SceneManager.h"

#include "Scene.h"

#include "GameObject.h"

#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Animation.h"
#include "Collider2D.h"

//Player
#include "RockManZ_Script.h"

//Enemy
#include "WalkCannon_Script.h"
#include "WalkCannon_Bullet_Script.h"

SceneManager::SceneManager()
{
	Initialize();
}

SceneManager::~SceneManager()
{
	m_p_current_scene.reset();
}

void SceneManager::Initialize()
{
	m_p_current_scene = std::make_shared<Scene>();

	//Camera
	auto camera = new GameObject();
	camera->SetObjectName("Main Camera");
	camera->SetObjectTag("Camera");
	camera->AddComponent(new Transform());
	camera->AddComponent(new Camera());

	camera->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, 0.0f, -5.0f));

	m_p_current_scene->AddGameObject(camera, 0, true);

	//RockManZ
	auto rockmanZ = new GameObject();
	rockmanZ->SetObjectName("RockManZ");
	rockmanZ->SetObjectTag("Player");
	rockmanZ->AddComponent(new Transform());
	rockmanZ->AddComponent(new Renderer());
	rockmanZ->AddComponent(new Animator());
	rockmanZ->AddComponent(new Collider2D());
	rockmanZ->AddComponent(new RockManZ_Script());

	auto collider2D = rockmanZ->GetComponent<Collider2D>();
	collider2D->SetOffsetPos(Vector3(0.0f, -10.0f, 0.0f));
	collider2D->SetOffsetScale(Vector3(0.4f, 0.5f, 1.0f));
	rockmanZ->GetComponent<Transform>()->SetScale(Vector3(1.5f, 1.5f, 1.5f));
	auto scale = rockmanZ->GetComponent<Transform>()->GetScale();
	rockmanZ->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, (scale.y * 33.0f), 0.0f));

	rockmanZ->GetComponent<Script>()->Initialize();
	rockmanZ->GetComponent<Animator>()->Play();

	m_p_current_scene->AddGameObject(rockmanZ, 1, false);

	//Enemy
	auto walkconnon = new GameObject();
	walkconnon->SetObjectName("WalkCannon");
	walkconnon->SetObjectTag("Enemy");
	walkconnon->AddComponent(new Transform());
	walkconnon->AddComponent(new Renderer());
	walkconnon->AddComponent(new Animator());
	walkconnon->AddComponent(new Collider2D());
	walkconnon->AddComponent(new WalkCannon_Script());

	collider2D = walkconnon->GetComponent<Collider2D>();
	collider2D->SetOffsetScale(Vector3(0.4f, 0.4f, 1.0f));

	walkconnon->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 2.0f));
	scale = walkconnon->GetComponent<Transform>()->GetScale();
	walkconnon->GetComponent<Transform>()->SetTranslation(Vector3(300.0f, (scale.y * 19.0f), 0.0f));

	walkconnon->GetComponent<Script>()->Initialize();
	walkconnon->GetComponent<Animator>()->Play();

	m_p_current_scene->AddGameObject(walkconnon, 2, false);
	CreatePrefab(walkconnon);

	//Enemy Bullet
	auto walkconnon_bullet = new GameObject();
	walkconnon_bullet->SetObjectName("WalkCannon_Bullet");
	walkconnon_bullet->SetObjectTag("Enemy");
	walkconnon_bullet->AddComponent(new Transform());
	walkconnon_bullet->AddComponent(new Renderer());
	walkconnon_bullet->AddComponent(new Animator());
	walkconnon_bullet->AddComponent(new Collider2D());
	walkconnon_bullet->AddComponent(new WalkCannon_Bullet_Script());

	collider2D = walkconnon_bullet->GetComponent<Collider2D>();
	collider2D->SetOffsetScale(Vector3(0.1f, 0.1f, 1.0f));
	
	walkconnon_bullet->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 2.0f));

	walkconnon_bullet->GetComponent<Script>()->Initialize();
	walkconnon_bullet->GetComponent<Animator>()->Play();

	m_p_current_scene->AddGameObject(walkconnon_bullet, 2, false);
	CreatePrefab(walkconnon_bullet);

	CollisionManager::GetInstance()->CheckLayer(1, 2);
}

void SceneManager::Progress()
{
	TimeManager::GetInstance()->Update();
	InputManager::GetInstance()->Update();

	m_p_current_scene->Update();
	m_p_current_scene->LateUpdate();
	m_p_current_scene->FinalUpdate();

	//충돌검사
	CollisionManager::GetInstance()->Update();

	m_p_current_scene->Render();
}

void SceneManager::CreatePrefab(GameObject* p_game_object)
{
	p_game_object->RegisterPrefab();
}
