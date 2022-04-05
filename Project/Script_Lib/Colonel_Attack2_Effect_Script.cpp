#include "stdafx.h"
#include "Colonel_Attack2_Effect_Script.h"

#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>

Colonel_Attack2_Effect_Script::Colonel_Attack2_Effect_Script()
	:Script("Colonel_Attack2_Effect_Script")
{
	RegisterScriptParamData();
}

Colonel_Attack2_Effect_Script::Colonel_Attack2_Effect_Script(const Colonel_Attack2_Effect_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	m_fire_speed = origin.m_fire_speed;
}

Colonel_Attack2_Effect_Script::~Colonel_Attack2_Effect_Script()
{
	m_p_collider2D = nullptr;
}

void Colonel_Attack2_Effect_Script::Start()
{
	ProjectileController::m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	ProjectileController::m_p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();

	m_p_collider2D = m_p_owner_game_object->GetComponent<Collider2D>();

	AddAnimationEvent();

	if (!m_p_animator2D->GetIsPlaying())
	{
		m_p_animator2D->Play();
	}
}

void Colonel_Attack2_Effect_Script::Update()
{
	UpdateSide();
	UpdateMove();
}

void Colonel_Attack2_Effect_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Fire Speed", ScriptParamType::Float, reinterpret_cast<void*>(&m_fire_speed), 100.0f));
}

void Colonel_Attack2_Effect_Script::AddAnimationEvent()
{
	auto animation_map = m_p_animator2D->GetAnimationMap();

	//Attack_2_Effect
	animation_map["Attack_2_Effect"]->SetAnimationEvent(0, std::bind(&Colonel_Attack2_Effect_Script::UpdateCollider2DSize, this));
	animation_map["Attack_2_Effect"]->SetAnimationEvent(1, std::bind(&Colonel_Attack2_Effect_Script::UpdateCollider2DSize, this));
	animation_map["Attack_2_Effect"]->SetAnimationEvent(2, std::bind(&Colonel_Attack2_Effect_Script::UpdateCollider2DSize, this));
	animation_map["Attack_2_Effect"]->SetAnimationEvent(3, std::bind(&Colonel_Attack2_Effect_Script::UpdateCollider2DSize, this));
	animation_map["Attack_2_Effect"]->SetAnimationEvent(4, std::bind(&Colonel_Attack2_Effect_Script::UpdateCollider2DSize, this));
	animation_map["Attack_2_Effect"]->SetAnimationEvent(5, std::bind(&Colonel_Attack2_Effect_Script::UpdateCollider2DSize, this));
	animation_map["Attack_2_Effect"]->SetAnimationEvent(6, std::bind(&Colonel_Attack2_Effect_Script::UpdateCollider2DSize, this));
}

void Colonel_Attack2_Effect_Script::UpdateCollider2DSize()
{
	switch (m_call_count)
	{
	case 0:
		m_p_collider2D->SetOffsetPosition(Vector2(0.2f, -0.43f));
		m_p_collider2D->SetOffsetScale(Vector2(0.1f, 0.25f));
		break;
	case 1:
		m_p_collider2D->SetOffsetPosition(Vector2(0.15f, -0.4f));
		m_p_collider2D->SetOffsetScale(Vector2(0.1f, 0.3f));
		break;
	case 2:
		m_p_collider2D->SetOffsetPosition(Vector2(0.15f, -0.35f));
		m_p_collider2D->SetOffsetScale(Vector2(0.1f, 0.42f));
		break;
	case 3:
		m_p_collider2D->SetOffsetPosition(Vector2(0.12f, -0.25f));
		m_p_collider2D->SetOffsetScale(Vector2(0.15f, 0.6f));
		break;
	case 4:
		m_p_collider2D->SetOffsetPosition(Vector2(0.0f, -0.15f));
		m_p_collider2D->SetOffsetScale(Vector2(0.2f, 0.8f));
		break;
	case 5:
		m_p_collider2D->SetOffsetPosition(Vector2(0.0f, -0.08f));
		m_p_collider2D->SetOffsetScale(Vector2(0.3f, 0.95f));
		break;
	case 6:
		m_p_collider2D->SetOffsetPosition(Vector2(-0.1f, 0.0f));
		m_p_collider2D->SetOffsetScale(Vector2(0.3f, 1.1f));
		break;
	}

	++m_call_count;
}

void Colonel_Attack2_Effect_Script::OnCollisionEnter(GameObject* p_other_game_object)
{
	//플레이어 또는 벽
	if (p_other_game_object->GetGameObjectTag()._Equal("Player") || p_other_game_object->GetGameObjectTag()._Equal("Wall"))
	{
		Destroy(m_p_owner_game_object);
	}
}

void Colonel_Attack2_Effect_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script

	ProjectileController::SaveToScene(p_file); //ProjectileController
}

void Colonel_Attack2_Effect_Script::LoadFromScene(FILE* p_file)
{
	ProjectileController::LoadFromScene(p_file); //ProjectileController
}
