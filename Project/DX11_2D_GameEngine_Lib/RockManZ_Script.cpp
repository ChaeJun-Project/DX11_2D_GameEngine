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
	auto animator = m_p_owner_game_object->GetComponent<Animator>();

	//Idle
	animator->CreateAnimation("RockManZ_Idle", "Texture/RockManZ/Animation/Z03_Idle/", 0.5f, true);
	animator->SetCurrentAnimation("RockManZ_Idle");

	//Walk_Begin
	animator->CreateAnimation("RockManZ_Walk_Begin", "Texture/RockManZ/Animation/Z06_Walk/Walk_Begin/", 0.5f, false);

	//Walk_Run
	animator->CreateAnimation("RockManZ_Walk_Run", "Texture/RockManZ/Animation/Z06_Walk/Walk_Run/", 0.5f, true);

	//Crouch
	animator->CreateAnimation("RockManZ_Crouch", "Texture/RockManZ/Animation/Z10_Crouch/Crouch/", 0.1f, false);

	//Attack
	animator->CreateAnimation("RockManZ_Attack", "Texture/RockManZ/Animation/Z04_Attack/Attack_1/", 0.5f, false);
}

void RockManZ_Script::Update()
{
	if (m_p_owner_game_object->GetObjectName() == "RockManZ")
	{
		auto animator = m_p_owner_game_object->GetComponent<Animator>();

		auto transform = m_p_owner_game_object->GetComponent<Transform>();

		auto input = InputManager::GetInstance();
		auto timer = TimeManager::GetInstance();

		Vector3 move_speed = Vector3::Zero;

		//오른쪽 이동
		if (input->KeyPress(KeyCode::KEY_ARROW_RIGHT))
		{
			move_speed.x += m_speed * timer->GetDeltaTime_float();
			transform->SetObjectSideState(GameObjectSideState::Right);
			animator->SetCurrentAnimation("RockManZ_Walk_Run");
			animator->Play();
		}

		//왼쪽이동
		if (input->KeyPress(KeyCode::KEY_ARROW_LEFT))
		{
			move_speed.x -= m_speed * timer->GetDeltaTime_float();
			transform->SetObjectSideState(GameObjectSideState::Left);
			animator->SetCurrentAnimation("RockManZ_Walk_Run");
			animator->Play();
		}

		//앉기
		if (input->KeyDown(KeyCode::KEY_ARROW_DOWN))
		{
			animator->SetCurrentAnimation("RockManZ_Crouch");
			animator->Play();
		}

		if (input->KeyDown(KeyCode::KEY_ARROW_UP))
		{
			animator->SetCurrentAnimation("RockManZ_Idle");
			animator->Play();
		}

		////대쉬
		//if (input->KeyPress(KeyCode::KEY_Z))
		//	//position -= m_speed * timer->GetDeltaTime_float();

		////점프
		//if (input->KeyPress(KeyCode::KEY_X))
		//	//position += m_speed * timer->GetDeltaTime_float();

		//공격
		if (input->KeyPress(KeyCode::KEY_C))
		{
			animator->SetCurrentAnimation("RockManZ_Attack");
			animator->Play();
		}

		//캐릭터 위치 변경
		transform->Translate(move_speed);
	}
}

void RockManZ_Script::Render()
{
}

void RockManZ_Script::OnCollisionEnter(GameObject* other_game_object)
{
}

void RockManZ_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void RockManZ_Script::OnCollision(GameObject* other_game_object)
{
}
