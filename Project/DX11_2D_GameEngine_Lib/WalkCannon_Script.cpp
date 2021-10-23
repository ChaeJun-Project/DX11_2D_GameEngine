#include "stdafx.h"
#include "WalkCannon_Script.h"

WalkCannon_Script::WalkCannon_Script()
{
}

WalkCannon_Script::~WalkCannon_Script()
{
}

void WalkCannon_Script::Initialize()
{
	auto animator = m_p_owner_game_object->GetComponent<Animator>();

	//====================================================================
	//[Idle]
	//====================================================================
	animator->CreateAnimation("WalkCannon_Idle", "Texture/Enemy/WalkCannon/Animation/WalkCannon_01_Idle/", 0.4f, true);
	animator->SetCurrentAnimation("WalkCannon_Idle");

	//====================================================================
	//[Fire]
	//====================================================================
	//LandFireMid
	animator->CreateAnimation("WalkCannon_LandFireMid", "Texture/Enemy/WalkCannon/Animation/WalkCannon_02_Fire/LandFireMid/", 0.2f, false);
	//LandFireTop
	animator->CreateAnimation("WalkCannon_LandFireTop", "Texture/Enemy/WalkCannon/Animation/WalkCannon_02_Fire/LandFireTop/", 0.2f, false);
	//LandFireBot
	animator->CreateAnimation("WalkCannon_LandFireBot", "Texture/Enemy/WalkCannon/Animation/WalkCannon_02_Fire/LandFireBot/", 0.2f, false);

	//====================================================================
	//[Walk]
	//====================================================================
	animator->CreateAnimation("WalkCannon_Walk", "Texture/Enemy/WalkCannon/Animation/WalkCannon_03_Walk/", 0.8f, true);

	//====================================================================
	//[Jump]
	//====================================================================
	//Jump_Begin
	animator->CreateAnimation("WalkCannon_Jump_Begin", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/Jump_Begin/", 0.5f, false);
	//Jump_Run
	animator->CreateAnimation("WalkCannon_Jump_Run", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/Jump_Run/", 0.5f, false);
	//JumpFireMid
	animator->CreateAnimation("WalkCannon_JumpFireMid", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/JumpFireMid/", 0.2f, false);
	//JumpFireTop
	animator->CreateAnimation("WalkCannon_JumpFireTop", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/JumpFireTop/", 0.2f, false);
	//JumpFireBot
	animator->CreateAnimation("WalkCannon_JumpFireBot", "Texture/Enemy/WalkCannon/Animation/WalkCannon_04_Jump/JumpFireBot/", 0.2f, false);
}

void WalkCannon_Script::Update()
{
}

void WalkCannon_Script::Render()
{
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
