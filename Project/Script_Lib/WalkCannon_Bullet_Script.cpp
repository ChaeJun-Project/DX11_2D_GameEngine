#include "stdafx.h"
#include "WalkCannon_Bullet_Script.h"

WalkCannon_Bullet_Script::WalkCannon_Bullet_Script()
	:Script("WalkCannon_Bullet_Script")
{
}

WalkCannon_Bullet_Script::WalkCannon_Bullet_Script(const WalkCannon_Bullet_Script& origin)
	: Script(origin.m_script_name)
{
}

WalkCannon_Bullet_Script::~WalkCannon_Bullet_Script()
{
	m_p_animator = nullptr;
}

void WalkCannon_Bullet_Script::Start()
{
	//auto animator = m_p_owner_game_object->GetComponent<Animator>();

	////Fire
	//animator->CreateAnimation("WalkCannon_Bullet_Fire", "Texture/Enemy/WalkCannon/Bullet/WalkCannon_Bullet_Fire/", 0.3f, true);
	//animator->SetCurrentAnimation("WalkCannon_Bullet_Fire");
}

void WalkCannon_Bullet_Script::Update()
{
	//if (m_p_animator == nullptr)
	//{ 
	//	m_p_animator = m_p_owner_game_object->GetComponent<Animator>();
	//	m_p_animator->Play();
	//}

	//auto timer = TimeManager::GetInstance();

	//auto transform = m_p_owner_game_object->GetComponent<Transform>();

	//auto position = transform->GetTranslation();

	//Vector3 move_speed = Vector3::Zero;

	//move_speed.x += m_speed * timer->GetDeltaTime_float();

	//move_speed = move_speed * m_fire_direction;

	////위치 변경
	//transform->SetTranslation(position + move_speed);
}

void WalkCannon_Bullet_Script::OnCollisionEnter(GameObject* other_game_object)
{
	/*if (other_game_object->GetGameObjectTag() == "Player")
	{
		Destroy(m_p_owner_game_object);
	}*/
}

void WalkCannon_Bullet_Script::OnCollisionStay(GameObject* other_game_object)
{
}

void WalkCannon_Bullet_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void WalkCannon_Bullet_Script::SetDirection(const Vector3& fire_direction)
{
	if(m_fire_direction == fire_direction)
	   return;

	m_fire_direction = fire_direction;
}

void WalkCannon_Bullet_Script::SaveToScene(FILE* p_file)
{
}

void WalkCannon_Bullet_Script::LoadFromScene(FILE* p_file)
{
}
