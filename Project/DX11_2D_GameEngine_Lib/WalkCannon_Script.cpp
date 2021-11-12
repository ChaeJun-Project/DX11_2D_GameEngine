#include "stdafx.h"
#include "WalkCannon_Script.h"

#include "Scene.h"

#include "Layer.h"

//Bullet
#include "WalkCannon_Bullet_Script.h"

WalkCannon_Script::WalkCannon_Script()
{
}

WalkCannon_Script::WalkCannon_Script(const WalkCannon_Script& origin)
	:Script(origin)
{
	this->m_p_player = origin.m_p_player;
	this->m_current_state = AnimationState::Idle;
	this->m_p_bullet_prefab = origin.m_p_bullet_prefab;
}

WalkCannon_Script::~WalkCannon_Script()
{
	m_p_animator = nullptr;
}

void WalkCannon_Script::Initialize()
{
	this->m_p_animator = m_p_owner_game_object->GetComponent<Animator>();

	//====================================================================
	//[Idle]
	//====================================================================
	this->m_p_animator->CreateAnimation("WalkCannon_Idle", "Texture/Enemy/WalkCannon/Animation/WalkCannon_01_Idle/", 0.4f, true);

	//====================================================================
	//[Fire]
	//====================================================================
	//LandFireMid
	this->m_p_animator->CreateAnimation("WalkCannon_LandFireMid", "Texture/Enemy/WalkCannon/Animation/WalkCannon_02_Fire/LandFireMid/", 0.2f, false);
	this->m_p_animator->SetAnimationEvent("WalkCannon_LandFireMid", 1, [=]() { CreateBullet(); });
	//LandFireTop
	this->m_p_animator->CreateAnimation("WalkCannon_LandFireTop", "Texture/Enemy/WalkCannon/Animation/WalkCannon_02_Fire/LandFireTop/", 0.2f, false);
	//LandFireBot
	this->m_p_animator->CreateAnimation("WalkCannon_LandFireBot", "Texture/Enemy/WalkCannon/Animation/WalkCannon_02_Fire/LandFireBot/", 0.2f, false);

	//====================================================================
	//[Walk]
	//====================================================================
	this->m_p_animator->CreateAnimation("WalkCannon_Walk", "Texture/Enemy/WalkCannon/Animation/WalkCannon_03_Walk/", 0.8f, true);

	//====================================================================
	//[Jump]
	//====================================================================
	//Jump_Begin
	this->m_p_animator->CreateAnimation("WalkCannon_Jump_Begin", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/Jump_Begin/", 0.5f, false);
	//Jump_Run
	this->m_p_animator->CreateAnimation("WalkCannon_Jump_Run", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/Jump_Run/", 0.5f, false);
	//JumpFireMid
	this->m_p_animator->CreateAnimation("WalkCannon_JumpFireMid", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/JumpFireMid/", 0.2f, false);
	//JumpFireTop
	this->m_p_animator->CreateAnimation("WalkCannon_JumpFireTop", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/JumpFireTop/", 0.2f, false);
	//JumpFireBot
	this->m_p_animator->CreateAnimation("WalkCannon_JumpFireBot", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/JumpFireBot/", 0.2f, false);

	this->m_p_animator->SetCurrentAnimation("WalkCannon_Idle");

	auto scene_manager = SceneManager::GetInstance();

	//Enemy Bullet(4)
	auto walkconnon_bullet = new GameObject();
	walkconnon_bullet->SetObjectName("WalkCannon_Bullet");
	walkconnon_bullet->SetObjectTag("Enemy");
	walkconnon_bullet->AddComponent(new Transform());
	walkconnon_bullet->AddComponent(new SpriteRenderer());
	walkconnon_bullet->AddComponent(new Animator());
	walkconnon_bullet->AddComponent(new Collider2D());
	walkconnon_bullet->AddComponent(new WalkCannon_Bullet_Script());

	auto collider2D = walkconnon_bullet->GetComponent<Collider2D>();
	collider2D->SetOffsetScale(Vector3(0.1f, 0.1f, 1.0f));

	walkconnon_bullet->GetComponent<Transform>()->SetScale(Vector3(2.0f, 2.0f, 2.0f));

	walkconnon_bullet->GetComponent<Script>()->Initialize();

	scene_manager->CreatePrefab(walkconnon_bullet);
	this->m_p_bullet_prefab = ResourceManager::GetInstance()->GetPrefab("WalkCannon_Bullet");

	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
	auto game_object_vector = current_scene->GetLayer(3)->GetGameParentObjects();
	this->m_p_player = game_object_vector[0];
}

void WalkCannon_Script::Update()
{
	if (this->m_p_animator == nullptr)
		this->m_p_animator = m_p_owner_game_object->GetComponent<Animator>();

	if (this->m_p_player == nullptr)
	{
		auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
		auto game_object_vector = current_scene->GetLayer(3)->GetGameParentObjects();
		this->m_p_player = game_object_vector[0];
	}

	auto timer = TimeManager::GetInstance();
	//시간 누적
	this->m_accumulate_time += timer->GetDeltaTime_float();

	auto player_position = this->m_p_player->GetComponent<Transform>()->GetLocalTranslation();

	auto transform = m_p_owner_game_object->GetComponent<Transform>();
	auto position = transform->GetLocalTranslation();

	Vector3 move_speed = Vector3::Zero;

	auto dis = Vector3::Distance(player_position, position);
	//탐색 범위 내부인 경우
	if (dis <= m_idle_range)
	{
		if (player_position.x < position.x)
		{
			transform->SetObjectSideState(GameObjectSideState::Right);
		}

		else
			transform->SetObjectSideState(GameObjectSideState::Left);

		if (this->m_accumulate_time >= this->m_fire_cooltime)
		{
			this->m_current_state = AnimationState::LandFireMid;
			this->m_accumulate_time = 0.0f;
		}
	}

	else
	{
		if (player_position.x < position.x)
		{
			transform->SetObjectSideState(GameObjectSideState::Right);
			move_speed.x -= m_speed * timer->GetDeltaTime_float();
		}

		else
		{
			transform->SetObjectSideState(GameObjectSideState::Left);
			move_speed.x += m_speed * timer->GetDeltaTime_float();
		}


		this->m_current_state = AnimationState::Walk;
	}

	if (this->m_current_state == AnimationState::LandFireMid)
		this->m_p_animator->SetCurrentAnimation("WalkCannon_LandFireMid");

	if (this->m_current_state == AnimationState::Walk)
		this->m_p_animator->SetCurrentAnimation("WalkCannon_Walk");

	if (this->m_current_state == AnimationState::Idle)
		this->m_p_animator->SetCurrentAnimation("WalkCannon_Idle");

	//캐릭터 위치 변경
	transform->Translate(move_speed);
	CollisionManager::GetInstance()->CheckLayer(3, 4);
}

void WalkCannon_Script::CreateBullet()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	auto position = transform->GetLocalTranslation();
	auto player_position = this->m_p_player->GetComponent<Transform>()->GetLocalTranslation();

	auto direction = player_position - position;
	direction.Normalize();

	auto bullet = Instantiate(this->m_p_bullet_prefab, position, 4, true);
	dynamic_cast<WalkCannon_Bullet_Script*>(bullet->GetComponent<Script>())->SetDirection(direction);

	this->m_current_state = AnimationState::Idle;
}

void WalkCannon_Script::OnCollisionEnter(GameObject* other_game_object)
{
}

void WalkCannon_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void WalkCannon_Script::OnCollision(GameObject* other_game_object)
{
}

