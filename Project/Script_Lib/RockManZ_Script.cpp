#include "stdafx.h"
#include "RockManZ_Script.h"

RockManZ_Script::RockManZ_Script()
{

}

RockManZ_Script::~RockManZ_Script()
{
	m_p_animator = nullptr;
}

void RockManZ_Script::Start()
{
	m_p_animator = m_p_owner_game_object->GetComponent<Animator>();

	//Idle
	m_p_animator->CreateAnimation("RockManZ_Idle", "Texture/Player/RockManZ/Animation/Z03_Idle/", 0.5f, true);
	m_p_animator->SetCurrentAnimation("RockManZ_Idle");

	//Walk_Begin
	m_p_animator->CreateAnimation("RockManZ_Walk_Begin", "Texture/Player/RockManZ/Animation/Z06_Walk/Walk_Begin/", 0.5f, false);

	//Walk_Run
	m_p_animator->CreateAnimation("RockManZ_Walk_Run", "Texture/Player/RockManZ/Animation/Z06_Walk/Walk_Run/", 0.5f, true);

	//Crouch
	m_p_animator->CreateAnimation("RockManZ_Crouch", "Texture/Player/RockManZ/Animation/Z10_Crouch/Crouch/", 0.1f, false);

	//Attack
	m_p_animator->CreateAnimation("RockManZ_Attack", "Texture/Player/RockManZ/Animation/Z04_Attack/Attack_1/", 0.4f, true);

	//Damaged
	m_p_animator->CreateAnimation("RockManZ_Damaged", "Texture/Player/RockManZ/Animation/Z09_Damaged/Damaged/", 0.5f, true);


	//Light2D
	//auto point_light2D = new GameObject();
	//point_light2D->SetGameObjectName("Light2D_Point");
	//point_light2D->SetGameObjectTag("Light2D_Point");
	//point_light2D->AddComponent(new Transform());
	//point_light2D->AddComponent(new Light2D());

	//auto point_light = point_light2D->GetComponent<Light2D>();
	//point_light->SetLightType(LightType::Point);
	//point_light->SetLightRange(2000.0f);
	//point_light->SetLightColor(Vector4::White);

	//m_p_owner_game_object->AddChild(point_light2D);
}

void RockManZ_Script::Update()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	auto position = transform->GetTranslation();

	auto timer = TimeManager::GetInstance();

	Vector3 move_speed = Vector3::Zero;

	//오른쪽 이동
	if (KEY_PRESS(KeyCode::KEY_ARROW_RIGHT))
	{
		m_current_state = AnimationState::Walk_Run;
		move_speed.x += m_speed * timer->GetDeltaTime_float();
		transform->SetObjectSideState(GameObjectSideState::Right);
		transform->SetRotation(Quaternion::Identity);
		m_p_animator->SetCurrentAnimation("RockManZ_Walk_Run");
	}

	//왼쪽이동
	if (KEY_PRESS(KeyCode::KEY_ARROW_LEFT))
	{
		m_current_state = AnimationState::Walk_Run;
		move_speed.x -= m_speed * timer->GetDeltaTime_float();
		transform->SetObjectSideState(GameObjectSideState::Left);
		transform->SetRotation(Quaternion::QuaternionFromEulerAngle(Vector3(0.0f, 180.0f, 0.0f)));
		m_p_animator->SetCurrentAnimation("RockManZ_Walk_Run");
	}

	//앉기
	if (KEY_DOWN(KeyCode::KEY_ARROW_DOWN))
	{
		m_current_state = AnimationState::Crouch;
		m_p_animator->SetCurrentAnimation("RockManZ_Crouch");
	}

	if (KEY_DOWN(KeyCode::KEY_ARROW_UP))
	{
		m_current_state = AnimationState::Idle;
		m_p_animator->SetCurrentAnimation("RockManZ_Idle");
	}

	////대쉬
	//if (input->KeyPress(KeyCode::KEY_Z))
	//	//position -= m_speed * timer->GetDeltaTime_float();

	////점프
	//if (input->KeyPress(KeyCode::KEY_X))
	//	//position += m_speed * timer->GetDeltaTime_float();

	//공격
	if (KEY_PRESS(KeyCode::KEY_C))
	{
		m_current_state = AnimationState::Attack_1;
		m_p_animator->SetCurrentAnimation("RockManZ_Attack");
	}


	if (m_current_state == AnimationState::Damaged)
	{
		m_p_animator->SetCurrentAnimation("RockManZ_Damaged");
	}

	if (m_current_state == AnimationState::Idle)
	{
		m_p_animator->SetCurrentAnimation("RockManZ_Idle");
	}


	//캐릭터 위치 변경
	transform->SetTranslation(position + move_speed);
}

void RockManZ_Script::OnCollisionEnter(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Enemy")
	{
		m_current_state = AnimationState::Damaged;
	}
}

void RockManZ_Script::OnCollisionExit(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Enemy")
	{
	    m_current_state = AnimationState::Idle;
	}
}

void RockManZ_Script::OnCollision(GameObject* other_game_object)
{

}
