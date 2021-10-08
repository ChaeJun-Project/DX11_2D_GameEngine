#include "stdafx.h"
#include "RockManZ_Script.h"

RockManZ_Script::RockManZ_Script(GameObject* p_game_object)
	:IScript(p_game_object)
{
	Initialize();
}

RockManZ_Script::~RockManZ_Script()
{
}

void RockManZ_Script::Initialize()
{
	auto animator = m_p_game_object->GetComponent<Animator>();
	auto animation_idle = std::make_shared<Animation>("RockManZ_Idle");
	auto result = animation_idle->LoadFromFile("Texture/RockManZ/Animation/Z03_Idle/");
	assert(result);
	if (result)
	{
		animation_idle->SetAnimationTime(3.0f);
		animation_idle->SetIsLoop(true);
		animator->AddAnimation(animation_idle->GetResourceName(), animation_idle);
	}

	animator->SetCurrentAnimation("RockManZ_Idle");
}

void RockManZ_Script::Update()
{
}

void RockManZ_Script::FinalUpdate()
{
}

void RockManZ_Script::Render()
{
}
