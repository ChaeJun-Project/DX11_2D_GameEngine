#include "stdafx.h"
#include "RockManZ_Script.h"

RockManZ_Script::RockManZ_Script()
{

}

RockManZ_Script::~RockManZ_Script()
{
	
}

void RockManZ_Script::Initialize()
{
	auto animator = m_p_owner_game_object.lock()->GetComponent<Animator>();
	animator->CreateAnimation("RockManZ_Attack", "Texture/RockManZ/Animation/Z04_Attack/Attack_1/", 0.5f, true);
	animator->SetCurrentAnimation("RockManZ_Attack");
}

void RockManZ_Script::Update()
{
}

void RockManZ_Script::Render()
{
}
