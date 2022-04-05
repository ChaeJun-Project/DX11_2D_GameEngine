#include "stdafx.h"
#include "Colonel_Attack1_Effect_Script.h"

#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>

Colonel_Attack1_Effect_Script::Colonel_Attack1_Effect_Script()
	:Script("Colonel_Attack1_Effect_Script")
{
	RegisterScriptParamData();
}

Colonel_Attack1_Effect_Script::Colonel_Attack1_Effect_Script(const Colonel_Attack1_Effect_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	m_fire_speed = origin.m_fire_speed;
}

void Colonel_Attack1_Effect_Script::Start()
{
	ProjectileController::m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	ProjectileController::m_p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();

	if (!m_p_animator2D->GetIsPlaying())
	{
		m_p_animator2D->SetIsLoop(true);
		m_p_animator2D->Play();
	}
}

void Colonel_Attack1_Effect_Script::Update()
{
	UpdateSide();
	UpdateMove();
}

void Colonel_Attack1_Effect_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Fire Speed", ScriptParamType::Float, reinterpret_cast<void*>(&m_fire_speed), 100.0f));
}

void Colonel_Attack1_Effect_Script::OnCollisionEnter(GameObject* p_other_game_object)
{
	//플레이어 또는 벽
	if (p_other_game_object->GetGameObjectTag()._Equal("Player") || p_other_game_object->GetGameObjectTag()._Equal("Wall"))
	{
		Destroy(m_p_owner_game_object);
	}
}

void Colonel_Attack1_Effect_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script

	ProjectileController::SaveToScene(p_file); //ProjectileController
}

void Colonel_Attack1_Effect_Script::LoadFromScene(FILE* p_file)
{
	ProjectileController::LoadFromScene(p_file); //ProjectileController
}
