#include "stdafx.h"
#include "Colonel_Script.h"

#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>

Colonel_Script::Colonel_Script()
	:Script("Colonel_Script")
{
	RegisterScriptParamData();

	m_pre_state = m_current_state;
}

Colonel_Script::Colonel_Script(const Colonel_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();
}

Colonel_Script::~Colonel_Script()
{
	m_p_transform = nullptr;
}

void Colonel_Script::Start()
{
	m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	GameObjectController::m_p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();

	AddAnimationEvent();

	SetCurrentAnimation("Colonel_Start");
}

void Colonel_Script::Update()
{
}

void Colonel_Script::Update_Move()
{
}

void Colonel_Script::Update_State()
{
}

void Colonel_Script::Update_Animation()
{
}

void Colonel_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Hp", ScriptParamType::Int, reinterpret_cast<void*>(&m_hp)));
	AddScriptParamData(ScriptParamStruct("Current State", ScriptParamType::Int, reinterpret_cast<void*>(&m_current_state)));
}

void Colonel_Script::AddAnimationEvent()
{
	auto animation_map = m_p_animator2D->GetAnimationMap();

	//Start
	animation_map["Colonel_Start"]->SetAnimationEvent(9, std::bind(&Colonel_Script::TriggerIdleState, this));
}

void Colonel_Script::TriggerIdleState()
{
	SetCurrentAnimation("Colonel_Idle", true);
	m_current_state = static_cast<UINT>(ColonelState::Idle);
}

void Colonel_Script::OnCollisionEnter(GameObject* other_game_object)
{
}

void Colonel_Script::OnCollisionStay(GameObject* other_game_object)
{
}

void Colonel_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void Colonel_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script

	GameObjectController::SaveToScene(p_file); //GameObjectController
}

void Colonel_Script::LoadFromScene(FILE* p_file)
{
	GameObjectController::LoadFromScene(p_file); //GameObjectController
}
