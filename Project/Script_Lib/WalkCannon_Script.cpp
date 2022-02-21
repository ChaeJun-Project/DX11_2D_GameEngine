#include "stdafx.h"
#include "WalkCannon_Script.h"

#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/Layer.h>

//Bullet
#include "WalkCannon_Bullet_Script.h"

WalkCannon_Script::WalkCannon_Script()
	:Script("WalkCannon_Script")
{
}

WalkCannon_Script::WalkCannon_Script(const WalkCannon_Script& origin)
	:Script(origin.m_script_name)
{
	m_p_player = origin.m_p_player;
	m_current_state = AnimationState::Idle;
	m_p_bullet_prefab = origin.m_p_bullet_prefab;
}

WalkCannon_Script::~WalkCannon_Script()
{
	//m_p_animator = nullptr;
}

void WalkCannon_Script::Start()
{
	//m_p_animator = m_p_owner_game_object->GetComponent<Animator>();

	////====================================================================
	////[Idle]
	////====================================================================
	//m_p_animator->CreateAnimation("WalkCannon_Idle", "Texture/Enemy/WalkCannon/Animation/WalkCannon_01_Idle/", 0.4f, true);

	////====================================================================
	////[Fire]
	////====================================================================
	////LandFireMid
	//m_p_animator->CreateAnimation("WalkCannon_LandFireMid", "Texture/Enemy/WalkCannon/Animation/WalkCannon_02_Fire/LandFireMid/", 0.2f, false);
	////m_p_animator->SetAnimationEvent("WalkCannon_LandFireMid", 1, [=]() { CreateBullet(); });
	////LandFireTop
	//m_p_animator->CreateAnimation("WalkCannon_LandFireTop", "Texture/Enemy/WalkCannon/Animation/WalkCannon_02_Fire/LandFireTop/", 0.2f, false);
	////LandFireBot
	//m_p_animator->CreateAnimation("WalkCannon_LandFireBot", "Texture/Enemy/WalkCannon/Animation/WalkCannon_02_Fire/LandFireBot/", 0.2f, false);

	////====================================================================
	////[Walk]
	////====================================================================
	//m_p_animator->CreateAnimation("WalkCannon_Walk", "Texture/Enemy/WalkCannon/Animation/WalkCannon_03_Walk/", 0.8f, true);

	////====================================================================
	////[Jump]
	////====================================================================
	////Jump_Begin
	//m_p_animator->CreateAnimation("WalkCannon_Jump_Begin", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/Jump_Begin/", 0.5f, false);
	////Jump_Run
	//m_p_animator->CreateAnimation("WalkCannon_Jump_Run", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/Jump_Run/", 0.5f, false);
	////JumpFireMid
	//m_p_animator->CreateAnimation("WalkCannon_JumpFireMid", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/JumpFireMid/", 0.2f, false);
	////JumpFireTop
	//m_p_animator->CreateAnimation("WalkCannon_JumpFireTop", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/JumpFireTop/", 0.2f, false);
	////JumpFireBot
	//m_p_animator->CreateAnimation("WalkCannon_JumpFireBot", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/JumpFireBot/", 0.2f, false);

	//m_p_animator->SetCurrentAnimation("WalkCannon_Idle");

	//m_p_bullet_prefab = ResourceManager::GetInstance()->GetResource<Prefab>("WalkCannon_Bullet");

	//auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
	//auto game_object_vector = current_scene->GetLayer(3)->GetParentGameObjects();
	//m_p_player = game_object_vector[0];

	//CollisionManager::GetInstance()->CheckLayer(3, 4);
}

void WalkCannon_Script::Update()
{
	//if (m_p_animator == nullptr)
	//	m_p_animator = m_p_owner_game_object->GetComponent<Animator>();

	//if (m_p_player == nullptr)
	//{
	//	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
	//	auto game_object_vector = current_scene->GetLayer(3)->GetParentGameObjects();
	//	m_p_player = game_object_vector[0];
	//}

	//auto timer = TimeManager::GetInstance();
	////시간 누적
	//m_accumulate_time += timer->GetDeltaTime_float();

	//auto player_position = m_p_player->GetComponent<Transform>()->GetLocalTranslation();

	//auto transform = m_p_owner_game_object->GetComponent<Transform>();

	//auto position = transform->GetTranslation();

	//Vector3 move_speed = Vector3::Zero;

	//auto dis = Vector3::Distance(player_position, position);
	////탐색 범위 내부인 경우
	//if (dis <= m_idle_range)
	//{
	//	if (player_position.x < position.x)
	//	{
	//		transform->SetObjectSideState(GameObjectSideState::Right);
	//	}

	//	else
	//		transform->SetObjectSideState(GameObjectSideState::Left);

	//	if (m_accumulate_time >= m_fire_cooltime)
	//	{
	//		m_current_state = AnimationState::LandFireMid;
	//		m_accumulate_time = 0.0f;
	//	}
	//}

	//else
	//{
	//	if (player_position.x < position.x)
	//	{
	//		transform->SetObjectSideState(GameObjectSideState::Right);
	//		move_speed.x -= m_speed * timer->GetDeltaTime_float();
	//	}

	//	else
	//	{
	//		transform->SetObjectSideState(GameObjectSideState::Left);
	//		move_speed.x += m_speed * timer->GetDeltaTime_float();
	//	}


	//	m_current_state = AnimationState::Walk;
	//}

	//if (m_current_state == AnimationState::LandFireMid)
	//	m_p_animator->SetCurrentAnimation("WalkCannon_LandFireMid");

	//if (m_current_state == AnimationState::Walk)
	//	m_p_animator->SetCurrentAnimation("WalkCannon_Walk");

	//if (m_current_state == AnimationState::Idle)
	//	m_p_animator->SetCurrentAnimation("WalkCannon_Idle");

	////캐릭터 위치 변경
	//transform->SetTranslation(position + move_speed);
}

void WalkCannon_Script::CreateBullet()
{
	/*auto transform = m_p_owner_game_object->GetComponent<Transform>();

	auto position = transform->GetLocalTranslation();
	auto player_position = m_p_player->GetComponent<Transform>()->GetLocalTranslation();

	auto direction = player_position - position;
	direction.Normalize();

	auto scene_manager = SceneManager::GetInstance();
	auto current_scene = scene_manager->GetCurrentScene();

	auto bullet = Instantiate(m_p_bullet_prefab, position, 4, true);
	
	current_scene->RegisterGameObject(bullet);
	dynamic_cast<WalkCannon_Bullet_Script*>(bullet->GetComponent<Script>())->SetDirection(direction);

	m_current_state = AnimationState::Idle;*/
}

void WalkCannon_Script::OnCollisionEnter(GameObject* other_game_object)
{
}

void WalkCannon_Script::OnCollisionStay(GameObject* other_game_object)
{
}

void WalkCannon_Script::OnCollisionExit(GameObject* other_game_object)
{
}
