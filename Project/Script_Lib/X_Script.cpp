#include "stdafx.h"
#include "X_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>
#include <DX11_2D_GameEngine_Lib/RigidBody2D.h>

X_Script::X_Script()
	:Script("X_Script")
{
	RegisterScriptParamData();
}

X_Script::X_Script(const X_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	m_is_active = origin.m_is_active;
}

X_Script::~X_Script()
{
	m_p_transform = nullptr;
	m_p_rigidbody2D = nullptr;
}

void X_Script::Awake()
{
}

void X_Script::Start()
{
	m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	GameObjectController::m_p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();
	m_p_rigidbody2D = m_p_owner_game_object->GetComponent<RigidBody2D>();
}

void X_Script::Update()
{
	auto position = m_p_transform->GetLocalTranslation();

	Vector3 movement_speed = Vector3::Zero;

	//오른쪽 이동
	if (KEY_PRESS(Key::KEY_ARROW_RIGHT))
	{
		movement_speed.x += m_run_speed * DELTA_TIME_F;
	}

	//왼쪽이동
	if (KEY_PRESS(Key::KEY_ARROW_LEFT))
	{
		movement_speed.x -= m_run_speed * DELTA_TIME_F;
	}

	//위 이동
	if (KEY_PRESS(Key::KEY_ARROW_UP))
	{
		movement_speed.y += m_run_speed * DELTA_TIME_F;
	}

	//아래 이동
	if (KEY_PRESS(Key::KEY_ARROW_DOWN))
	{
		movement_speed.y -= m_run_speed * DELTA_TIME_F;
	}


	m_p_transform->SetLocalTranslation(position + movement_speed);
}

void X_Script::Update_Move()
{
}

void X_Script::Update_State()
{
}

void X_Script::Update_Animation()
{
}

void X_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Hp", ScriptParamType::Int, reinterpret_cast<void*>(&m_hp), 100.0f));
	AddScriptParamData(ScriptParamStruct("Run Speed", ScriptParamType::Float, reinterpret_cast<void*>(&m_run_speed), 100.0f));
	AddScriptParamData(ScriptParamStruct("Jump Speed", ScriptParamType::Float, reinterpret_cast<void*>(&m_jump_speed), 100.0f));
	AddScriptParamData(ScriptParamStruct("Dash Speed", ScriptParamType::Float, reinterpret_cast<void*>(&m_dash_speed), 100.0f));
}

void X_Script::OnCollisionEnter(GameObject* other_game_object)
{
}

void X_Script::OnCollisionStay(GameObject* other_game_object)
{
}

void X_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void X_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script

	GameObjectController::SaveToScene(p_file); //GameObjectController
}

void X_Script::LoadFromScene(FILE* p_file)
{
	GameObjectController::LoadFromScene(p_file); //GameObjectController
}
