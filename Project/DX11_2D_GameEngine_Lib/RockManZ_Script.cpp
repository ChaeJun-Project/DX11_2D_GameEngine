#include "stdafx.h"
#include "RockManZ_Script.h"

RockManZ_Script::RockManZ_Script(GameObject* p_game_object)
	:IScript(p_game_object)
{
	Initialize();
}

RockManZ_Script::~RockManZ_Script()
{
	m_p_game_object.reset();
}

void RockManZ_Script::Initialize()
{
	auto animator = m_p_game_object->GetComponent<Animator>();
	animator->CreateAnimation("RockManZ_Idle", "Texture/RockManZ/Animation/Z03_Idle/", 3.0f);
	animator->SetCurrentAnimation("RockManZ_Idle");
}

void RockManZ_Script::Update()
{
}

void RockManZ_Script::Render()
{
}
