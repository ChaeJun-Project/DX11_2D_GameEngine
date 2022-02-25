#include "stdafx.h"
#include "Z_Script.h"

#include <DX11_2D_GameEngine_Lib/Texture.h>
#include <DX11_2D_GameEngine_Lib/Prefab.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>

Z_Script::Z_Script()
	:Script("Z_Script")
{
	AddScriptParamData(ScriptParamStruct("Speed", ScriptParamType::Float, reinterpret_cast<void*>(&m_speed)));
	//Test
	AddScriptParamData(ScriptParamStruct("Test1", ScriptParamType::Int, reinterpret_cast<void*>(&test1)));
	AddScriptParamData(ScriptParamStruct("Test2", ScriptParamType::Vector2, reinterpret_cast<void*>(&test2)));
	AddScriptParamData(ScriptParamStruct("Test3", ScriptParamType::Vector3, reinterpret_cast<void*>(&test3)));
	AddScriptParamData(ScriptParamStruct("Test4", ScriptParamType::Vector4, reinterpret_cast<void*>(&test4)));
	AddScriptParamData(ScriptParamStruct("Test5", ScriptParamType::Texture, reinterpret_cast<void*>(&p_texture)));
	AddScriptParamData(ScriptParamStruct("Test6", ScriptParamType::Prefab, reinterpret_cast<void*>(&p_prefab)));
}

Z_Script::Z_Script(const Z_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;

	m_speed = origin.m_speed;

}

Z_Script::~Z_Script()
{
	m_p_animator = nullptr;

	p_texture = nullptr;
	p_prefab = nullptr;
}

void Z_Script::Initialize()
{
	m_p_animator->SetCurrentAnimation("Z_Idle");
}

void Z_Script::Start()
{
	m_p_animator = m_p_owner_game_object->GetComponent<Animator2D>();
	m_p_animator->SetCurrentAnimation("Z_Idle");
	m_p_animator->SetIsLoop(true);
}

void Z_Script::Update()
{
	if (m_p_animator == nullptr)
		return;

	auto transform = m_p_owner_game_object->GetComponent<Transform>();
	auto position = transform->GetLocalTranslation();

	Vector3 move_speed = Vector3::Zero;

	//Test
	if (KEY_DOWN(KeyCode::KEY_K))
	{
		if (p_prefab != nullptr)
		{
			Instantiate(p_prefab, Vector3(0.0f, 0.0f, 0.0f), true);
		}
	}

	//오른쪽 이동
	if (KEY_PRESS(KeyCode::KEY_ARROW_RIGHT))
	{
		m_current_state = AnimationState::Walk_Run;
		move_speed.x += m_speed * TIME_MANAGER->GetDeltaTime_float();
		transform->SetRotation(Quaternion::Identity);
		m_p_animator->SetCurrentAnimation("Z_Walk_Run");
		m_p_animator->SetIsLoop(true);
	}

	//왼쪽이동
	else if (KEY_PRESS(KeyCode::KEY_ARROW_LEFT))
	{
		m_current_state = AnimationState::Walk_Run;
		move_speed.x -= m_speed * TIME_MANAGER->GetDeltaTime_float();
		transform->SetRotation(Quaternion::QuaternionFromEulerAngle(Vector3(0.0f, 180.0f, 0.0f)));
		m_p_animator->SetCurrentAnimation("Z_Walk_Run");
		m_p_animator->SetIsLoop(true);
	}

	//앉기
	if (KEY_DOWN(KeyCode::KEY_ARROW_DOWN))
	{
		m_current_state = AnimationState::Crouch;
		m_p_animator->SetCurrentAnimation("Z_Crouch");
	}

	//대쉬
	if (KEY_PRESS(KeyCode::KEY_Z))
		position -= m_speed * TIME_MANAGER->GetDeltaTime_float();

	//점프
	if (KEY_PRESS(KeyCode::KEY_X))
		position += m_speed * TIME_MANAGER->GetDeltaTime_float();

	//공격
	if (KEY_PRESS(KeyCode::KEY_C))
	{
		m_current_state = AnimationState::Attack_1;
		m_p_animator->SetCurrentAnimation("Z_Attack_1");
	}

	//캐릭터 위치 변경
	transform->SetTranslation(position + move_speed);
}

void Z_Script::OnCollisionEnter(GameObject* other_game_object)
{
	/*if (other_game_object->GetGameObjectTag() == "Enemy")
	{
		m_current_state = AnimationState::Damaged;
	}*/
}

void Z_Script::OnCollisionStay(GameObject* other_game_object)
{

}

void Z_Script::OnCollisionExit(GameObject* other_game_object)
{
	/*if (other_game_object->GetGameObjectTag() == "Enemy")
	{
		m_current_state = AnimationState::Idle;
	}*/
}

void Z_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script

	//Speed
	fprintf_s(p_file, "%f\n", m_speed);
}

void Z_Script::LoadFromScene(FILE* p_file)
{
	//Speed
	fscanf_s(p_file, "%f\n", &m_speed);
}
