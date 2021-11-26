#include "stdafx.h"
#include "GameLogic_Script.h"

#include "Scene.h"

//Geometry
#include "Geometry_script.h"

//Player
#include "RockManZ_Script.h"

//Enemy
#include "WalkCannon_Script.h"
#include "WalkCannon_Bullet_Script.h"

GameLogic_Script::GameLogic_Script()
{
}

GameLogic_Script::~GameLogic_Script()
{
}

void GameLogic_Script::Initialize()
{
	auto scene_manager = SceneManager::GetInstance();
	auto current_scene = scene_manager->GetCurrentScene();

	//Geometry(2)
	auto geometry = new GameObject();
	geometry->SetObjectName("Geometry");
	geometry->SetObjectTag("Geometry");
	geometry->AddComponent(new Transform());
	geometry->AddComponent(new Geometry_script());

	current_scene->AddGameObject(geometry, 2, true);
	geometry->GetComponent<Script>()->Initialize();

	//RockManZ(3)
	auto rockmanZ = new GameObject();
	rockmanZ->SetObjectName("RockManZ");
	rockmanZ->SetObjectTag("Player");
	rockmanZ->AddComponent(new Transform());
	rockmanZ->AddComponent(new SpriteRenderer());
	rockmanZ->AddComponent(new Animator());
	rockmanZ->AddComponent(new Collider2D());
	rockmanZ->AddComponent(new RockManZ_Script());

	auto collider2D = rockmanZ->GetComponent<Collider2D>();
	collider2D->SetOffsetPosition(Vector2(0.0f, -0.1f));
	collider2D->SetOffsetScale(Vector2(0.45f, 0.5f));
	rockmanZ->GetComponent<Transform>()->SetScale(Vector3(1.5f, 1.5f, 1.5f));
	auto scale = rockmanZ->GetComponent<Transform>()->GetScale();
	rockmanZ->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, (scale.y * 33.0f - 180.0f), 0.0f));

	rockmanZ->GetComponent<Script>()->Initialize();
	current_scene->AddGameObject(rockmanZ, 3, true);

	//Enemy(4)
	auto walkcannon = new GameObject();
	walkcannon->SetObjectName("WalkCannon");
	walkcannon->SetObjectTag("Enemy");
	walkcannon->AddComponent(new Transform());
	walkcannon->AddComponent(new SpriteRenderer());
	walkcannon->AddComponent(new Animator());
	walkcannon->AddComponent(new Collider2D());
	walkcannon->AddComponent(new WalkCannon_Script());

	collider2D = walkcannon->GetComponent<Collider2D>();
	collider2D->SetOffsetScale(Vector2(0.4f, 0.4f));

	walkcannon->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 2.0f));
	scale = walkcannon->GetComponent<Transform>()->GetScale();
	walkcannon->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, (scale.y * 19.0f), 0.0f));
	walkcannon->GetComponent<Transform>()->SetTranslation(Vector3(300.f, -160.f, 0.0f));

	scene_manager->CreatePrefab(walkcannon);
	m_p_walkcannon_prefab = ResourceManager::GetInstance()->GetPrefab("WalkCannon");

	//Enemy Bullet(4)
	auto walkconnon_bullet = new GameObject();
	walkconnon_bullet->SetObjectName("WalkCannon_Bullet");
	walkconnon_bullet->SetObjectTag("Enemy");
	walkconnon_bullet->AddComponent(new Transform());
	walkconnon_bullet->AddComponent(new SpriteRenderer());
	walkconnon_bullet->AddComponent(new Animator());
	walkconnon_bullet->AddComponent(new Collider2D());
	walkconnon_bullet->AddComponent(new WalkCannon_Bullet_Script());

	collider2D = walkconnon_bullet->GetComponent<Collider2D>();
	collider2D->SetOffsetScale(Vector2(0.1f, 0.1f));

	walkconnon_bullet->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 2.0f));

	scene_manager->CreatePrefab(walkconnon_bullet);

	auto collision_manager = CollisionManager::GetInstance();

	collision_manager->CheckLayer(2, 3);
}

void GameLogic_Script::Update()
{
	//auto scene_manager = SceneManager::GetInstance();
	//auto current_scene = scene_manager->GetCurrentScene();

	////시간 누적
	//m_accumulate_time += TimeManager::GetInstance()->GetDeltaTime_float();

	//if (m_accumulate_time > 10.0f)
	//{
	//	m_accumulate_time = 0.0f;

	//	
	//	auto walkcannon = Instantiate(m_p_walkcannon_prefab, Vector3(300.f, -300.f, 0.0f), 4, true);
	//	current_scene->AddGameObject(walkcannon, 4, true);
	//	auto scale = walkcannon->GetComponent<Transform>()->GetScale();
	//	walkcannon->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, (scale.y * 19.0f), 0.0f));
	//	walkcannon->GetComponent<Transform>()->SetTranslation(Vector3(300.f, -160.f, 0.0f));
	//	walkcannon->GetComponent<Script>()->Initialize();
	//}
}

void GameLogic_Script::OnCollisionEnter(GameObject* other_game_object)
{
}

void GameLogic_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void GameLogic_Script::OnCollision(GameObject* other_game_object)
{
}
