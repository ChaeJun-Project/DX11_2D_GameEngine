#include "stdafx.h"
#include "GameLogic_Script.h"

#include "Scene.h"

//Geometry
#include "Geometry_script.h"

//Player
#include "RockManZ_Script.h"

//Enemy
#include "WalkCannon_Script.h"

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

	geometry->GetComponent<Script>()->Initialize();
	current_scene->AddGameObject(geometry, 2, true);

	//RockManZ(3)
	auto rockmanZ = new GameObject();
	rockmanZ->SetObjectName("RockManZ");
	rockmanZ->SetObjectTag("Player");
	rockmanZ->AddComponent(new Transform());
	rockmanZ->AddComponent(new Renderer());
	rockmanZ->AddComponent(new Animator());
	rockmanZ->AddComponent(new Collider2D());
	rockmanZ->AddComponent(new RockManZ_Script());

	auto collider2D = rockmanZ->GetComponent<Collider2D>();
	collider2D->SetOffsetPos(Vector3(0.0f, -0.1f, 0.0f));
	collider2D->SetOffsetScale(Vector3(0.4f, 0.5f, 1.0f));
	rockmanZ->GetComponent<Transform>()->SetScale(Vector3(1.5f, 1.5f, 1.5f));
	auto scale = rockmanZ->GetComponent<Transform>()->GetScale();
	rockmanZ->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, (scale.y * 33.0f - 321.5f), 0.0f));

	rockmanZ->GetComponent<Script>()->Initialize();
	current_scene->AddGameObject(rockmanZ, 3, true);

	//Enemy(4)
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
	walkconnon->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, (scale.y * 19.0f), 0.0f));
	walkconnon->GetComponent<Transform>()->SetTranslation(Vector3(300.f, -300.f, 0.0f));

	walkconnon->GetComponent<Script>()->Initialize();
	current_scene->AddGameObject(walkconnon, 4, false);
	
	//scene_manager->CreatePrefab(walkconnon);
	//this->m_p_walkman_prefab = ResourceManager::GetInstance()->GetPrefab("WalkCannon");

	auto collision_manager = CollisionManager::GetInstance();

	//Instantiate(this->m_p_walkman_prefab, Vector3(300.f, -300.f, 0.0f), 4, true);
    //Instantiate(this->m_p_walkman_prefab, Vector3(-300.f, -300.f, 0.0f), 4, true);
	collision_manager->CheckLayer(2, 3);
}

void GameLogic_Script::Update()
{
	////시간 누적
	//this->m_accumulate_time += TimeManager::GetInstance()->GetDeltaTime_float();

	//if (this->m_accumulate_time >= 10.0f)
	//{
	//	this->m_accumulate_time = 0.0f;
	//	
	//	
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
