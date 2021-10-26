#include "stdafx.h"
#include "WalkCannon_Bullet_Script.h"

WalkCannon_Bullet_Script::~WalkCannon_Bullet_Script()
{
	this->m_p_animator = nullptr;
}

void WalkCannon_Bullet_Script::Initialize()
{
	auto animator = m_p_owner_game_object->GetComponent<Animator>();

	//Fire
	animator->CreateAnimation("WalkCannon_Bullet_Fire", "Texture/Enemy/WalkCannon/Bullet/WalkCannon_Bullet_Fire/", 0.3f, true);
	animator->SetCurrentAnimation("WalkCannon_Bullet_Fire");
}

void WalkCannon_Bullet_Script::Update()
{
	if (this->m_p_animator == nullptr)
	{ 
		this->m_p_animator = m_p_owner_game_object->GetComponent<Animator>();
		this->m_p_animator->Play();
	}

	auto timer = TimeManager::GetInstance();

	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	Vector3 move_speed = Vector3::Zero;

	move_speed.x += m_speed * timer->GetDeltaTime_float();

	move_speed = move_speed * this->m_fire_direction;

	//위치 변경
	transform->Translate(move_speed);
}

void WalkCannon_Bullet_Script::OnCollisionEnter(GameObject* other_game_object)
{
	if (other_game_object->GetObjectTag() == "Player")
	{
		Destroy(m_p_owner_game_object);
	}
}

void WalkCannon_Bullet_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void WalkCannon_Bullet_Script::OnCollision(GameObject* other_game_object)
{
}

void WalkCannon_Bullet_Script::SetDirection(const Vector3& fire_direction)
{
	if(this->m_fire_direction == fire_direction)
	   return;

	this->m_fire_direction = fire_direction;
}
