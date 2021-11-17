#include "stdafx.h"
#include "RockManZ_Script.h"

RockManZ_Script::RockManZ_Script()
{

}

RockManZ_Script::~RockManZ_Script()
{
	this->m_p_animator = nullptr;
}

void RockManZ_Script::Initialize()
{
	this->m_p_animator = m_p_owner_game_object->GetComponent<Animator>();

	//Idle
	this->m_p_animator->CreateAnimation("RockManZ_Idle", "Texture/Player/RockManZ/Animation/Z03_Idle/", 0.5f, true);
	this->m_p_animator->SetCurrentAnimation("RockManZ_Idle");

	//Walk_Begin
	this->m_p_animator->CreateAnimation("RockManZ_Walk_Begin", "Texture/Player/RockManZ/Animation/Z06_Walk/Walk_Begin/", 0.5f, false);

	//Walk_Run
	this->m_p_animator->CreateAnimation("RockManZ_Walk_Run", "Texture/Player/RockManZ/Animation/Z06_Walk/Walk_Run/", 0.5f, true);

	//Crouch
	this->m_p_animator->CreateAnimation("RockManZ_Crouch", "Texture/Player/RockManZ/Animation/Z10_Crouch/Crouch/", 0.1f, false);

	//Attack
	this->m_p_animator->CreateAnimation("RockManZ_Attack", "Texture/Player/RockManZ/Animation/Z04_Attack/Attack_1/", 0.4f, true);

	//Damaged
	this->m_p_animator->CreateAnimation("RockManZ_Damaged", "Texture/Player/RockManZ/Animation/Z09_Damaged/Damaged/", 0.5f, true);


	//Light2D
	auto point_light2D = new GameObject();
	point_light2D->SetObjectName("Light2D_Point");
	point_light2D->SetObjectTag("Light2D_Point");
	point_light2D->AddComponent(new Transform());
	point_light2D->AddComponent(new Light2D());

	auto point_light = point_light2D->GetComponent<Light2D>();
	point_light->SetLightType(LightType::Point);
	point_light->SetLightRange(2000.0f);
	point_light->SetLightColor(Color4::White);

	m_p_owner_game_object->AddChild(point_light2D);

	////Distortion
	//auto distortion = new GameObject();
	//distortion->SetObjectName("Distortion");
	//distortion->SetObjectTag("Distortion");
	//distortion->AddComponent(new Transform());
	//distortion->AddComponent(new SpriteRenderer());

	//distortion->GetComponent<Transform>()->SetScale(Vector3(0.6f, 0.5f, 1.0f));

	//auto resource_manager = ResourceManager::GetInstance();
	//auto sprite_renderer = distortion->GetComponent<SpriteRenderer>();
	//sprite_renderer->SetMesh(resource_manager->GetMesh(MeshType::Rectangle));
	//sprite_renderer->SetMaterial(resource_manager->GetMaterial("PostEffect"));

	//m_p_owner_game_object->AddChild(distortion);
}

void RockManZ_Script::Update()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	auto input = InputManager::GetInstance();
	auto timer = TimeManager::GetInstance();

	Vector3 move_speed = Vector3::Zero;

	//������ �̵�
	if (input->KeyPress(KeyCode::KEY_ARROW_RIGHT))
	{
		this->m_current_state = AnimationState::Walk_Run;
		move_speed.x += m_speed * timer->GetDeltaTime_float();
		transform->SetObjectSideState(GameObjectSideState::Right);
		this->m_p_animator->SetCurrentAnimation("RockManZ_Walk_Run");
	}

	//�����̵�
	if (input->KeyPress(KeyCode::KEY_ARROW_LEFT))
	{
		this->m_current_state = AnimationState::Walk_Run;
		move_speed.x -= m_speed * timer->GetDeltaTime_float();
		transform->SetObjectSideState(GameObjectSideState::Left);
		this->m_p_animator->SetCurrentAnimation("RockManZ_Walk_Run");
	}

	//�ɱ�
	if (input->KeyDown(KeyCode::KEY_ARROW_DOWN))
	{
		this->m_current_state = AnimationState::Crouch;
		this->m_p_animator->SetCurrentAnimation("RockManZ_Crouch");
	}

	if (input->KeyDown(KeyCode::KEY_ARROW_UP))
	{
		this->m_current_state = AnimationState::Idle;
		this->m_p_animator->SetCurrentAnimation("RockManZ_Idle");
	}

	////�뽬
	//if (input->KeyPress(KeyCode::KEY_Z))
	//	//position -= m_speed * timer->GetDeltaTime_float();

	////����
	//if (input->KeyPress(KeyCode::KEY_X))
	//	//position += m_speed * timer->GetDeltaTime_float();

	//����
	if (input->KeyPress(KeyCode::KEY_C))
	{
		this->m_current_state = AnimationState::Attack_1;
		this->m_p_animator->SetCurrentAnimation("RockManZ_Attack");
	}


	if (this->m_current_state == AnimationState::Damaged)
	{
		this->m_p_animator->SetCurrentAnimation("RockManZ_Damaged");
	}

	if (this->m_current_state == AnimationState::Idle)
	{
		this->m_p_animator->SetCurrentAnimation("RockManZ_Idle");
	}


	//ĳ���� ��ġ ����
	transform->Translate(move_speed);
}

void RockManZ_Script::OnCollisionEnter(GameObject* other_game_object)
{
	if (other_game_object->GetObjectTag() == "Enemy")
	{
		this->m_current_state = AnimationState::Damaged;
	}
}

void RockManZ_Script::OnCollisionExit(GameObject* other_game_object)
{
	if (other_game_object->GetObjectTag() == "Enemy")
	{
	    this->m_current_state = AnimationState::Idle;
	}
}

void RockManZ_Script::OnCollision(GameObject* other_game_object)
{

}
