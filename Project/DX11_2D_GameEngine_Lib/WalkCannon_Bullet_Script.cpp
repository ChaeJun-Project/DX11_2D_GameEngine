#include "stdafx.h"
#include "WalkCannon_Bullet_Script.h"

WalkCannon_Bullet_Script::WalkCannon_Bullet_Script()
{
}

WalkCannon_Bullet_Script::~WalkCannon_Bullet_Script()
{
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

}


void WalkCannon_Bullet_Script::Render()
{
}

void WalkCannon_Bullet_Script::OnCollisionEnter(GameObject* other_game_object)
{
}

void WalkCannon_Bullet_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void WalkCannon_Bullet_Script::OnCollision(GameObject* other_game_object)
{
}
