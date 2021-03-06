#include "stdafx.h"
#include "Z_Script.h"

#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>
#include <DX11_2D_GameEngine_Lib/RigidBody2D.h>

Z_Script::Z_Script()
	:Script("Z_Script")
{
	RegisterScriptParamData();

	InitializePlayerStateDetailMap();

	m_pre_state = m_current_state;

}

Z_Script::Z_Script(const Z_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	InitializePlayerStateDetailMap();

	m_is_active = origin.m_is_active;

	m_hp = origin.m_hp;
	m_run_speed = origin.m_run_speed;
	m_jump_speed = origin.m_jump_speed;
	m_dash_speed = origin.m_dash_speed;
}

Z_Script::~Z_Script()
{
	m_p_rigidbody2D = nullptr;
	m_p_attack_hit_box = nullptr;

	p_dark_game_object = nullptr;
	p_distortion_game_object = nullptr;
}

void Z_Script::Awake()
{
	GameObjectController::m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	GameObjectController::m_p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();
	m_p_rigidbody2D = m_p_owner_game_object->GetComponent<RigidBody2D>();

	auto p_attack_hit = m_p_owner_game_object->GetChildFromIndex(0);
	m_p_attack_hit_box = p_attack_hit->GetComponent<Collider2D>();

	auto p_debuffs = m_p_owner_game_object->GetChildFromIndex(1);
	p_dark_game_object = p_debuffs->GetChildFromIndex(0);
	p_distortion_game_object = p_debuffs->GetChildFromIndex(1);

	AddAnimationEvent();
}

void Z_Script::Start()
{
	SetCurrentAnimation("Z_Ready");
}

void Z_Script::Update()
{
	if (m_is_ready)
	{
		Update_Move();
		Update_State();
		Update_Animation();

		if (is_jump)
		{
			if (m_p_rigidbody2D->GetIsGround())
			{
				is_jump = false;
				m_jump_count = 0;
				SetCurrentAnimation("Z_Fall_End");
			}

			if (is_jump_run && m_p_rigidbody2D->GetVelocity().y <= 0.0f)
				is_jump_run = false;
		}

		if ((m_pre_state & PlayerState::Attack) && !(m_current_state & PlayerState::Attack))
		{
			m_attack_count = 0;
			m_next_attck_time_limit = NEXT_ATTACK_TIME_LIMIT;
		}

		if (!(m_pre_state & PlayerState::Attack))
		{
			m_p_attack_hit_box->SetIsActive(false);
		}

		if (is_hit)
		{
			hit_delay -= DELTA_TIME_F;
			if (hit_delay <= 0.0f)
			{
				hit_delay = 2.0f;
				is_hit = false;
			}
		}

		if (m_is_dark)
		{
			m_debuff_dark_duration -= DELTA_TIME_F;
			if (m_debuff_dark_duration <= 0.0f)
			{
				m_debuff_dark_duration = DEBUFF_DARK_DURATION;
				m_is_dark = false;
				p_dark_game_object->SetIsActive(false);

				LOG_INFO_F("?????????? ???? ???????? ????????????.");
			}
		}

		if (m_is_distortion)
		{
			m_debuff_distortion_duration -= DELTA_TIME_F;
			if (m_debuff_distortion_duration <= 0.0f)
			{
				m_debuff_distortion_duration = DEBUFF_DISTORTION_DURATION;
				m_is_distortion = false;
				p_distortion_game_object->SetIsActive(false);

				LOG_INFO_F("?????????? ???? ???????? ????????????.");
			}
		}

		m_pre_state = m_current_state;

		if (m_hp == 0)
		{
			m_is_dead = true;
			m_is_ready = false;
			SetCurrentAnimation("Z_Damaged");
			p_dark_game_object->SetIsActive(false);
			p_distortion_game_object->SetIsActive(false);

			LOG_INFO_F("?????????? ?????? 0?? ??????????.");
		}
	}

	if (m_is_dead)
	{
		m_dead_event_call_wait -= DELTA_TIME_F;
		if (m_dead_event_call_wait <= 0.0f)
		{ 
			m_p_dead_event_func();
			m_is_dead = false;

			LOG_INFO_F("?????????? ?????? 0?? ???? ?????? ??????????????.");
		}
	}
}

void Z_Script::Update_Move()
{
	if (m_p_transform == nullptr || m_p_rigidbody2D == nullptr)
		return;

	//?????? ????
	if (KEY_PRESS(Key::KEY_ARROW_RIGHT))
	{
		if (m_pre_state == PlayerState::Attack || m_pre_state & PlayerState::Dash)
			return;

		m_side_state = SideState::Right; //GameObject?? ?????? ???????? ????
		if (!(m_pre_state & PlayerState::Jump))
			m_current_state = PlayerState::Walk;
		if (!m_p_rigidbody2D->GetIsRightWall())
			m_p_rigidbody2D->SetVelocity(Vector2(m_run_speed, m_p_rigidbody2D->GetVelocity().y));
	}

	//????????
	if (KEY_PRESS(Key::KEY_ARROW_LEFT))
	{
		if (m_pre_state == PlayerState::Attack || m_pre_state & PlayerState::Dash)
			return;

		m_side_state = SideState::Left; //GameObject?? ???? ???????? ????

		if (!(m_pre_state & PlayerState::Jump))
			m_current_state = PlayerState::Walk;
		if (!m_p_rigidbody2D->GetIsLeftWall())
			m_p_rigidbody2D->SetVelocity(Vector2(-m_run_speed, m_p_rigidbody2D->GetVelocity().y));
	}

	//???????? ?????? ???? ????
	//??????
	if (m_side_state == SideState::Right)
		m_p_transform->SetRotation(Quaternion::Identity);

	//????
	else
		m_p_transform->SetRotation(Quaternion::QuaternionFromEulerAngle(Vector3(0.0f, 180.0f, 0.0f)));

	if (!is_dash)
	{
		//????
		if (KEY_DOWN(Key::KEY_SPACE))
		{
			is_dash = true;
		}
	}

	//???? ?????? ?????? 
	if (!is_jump)
	{
		//????
		if (KEY_DOWN(Key::KEY_ALT))
		{
			is_jump = true;
			m_p_rigidbody2D->SetVelocity(Vector2(m_p_rigidbody2D->GetVelocity().x, m_jump_speed));
			m_p_rigidbody2D->SetGround(false);
		}
	}
}

void Z_Script::Update_State()
{
	if (m_p_rigidbody2D->GetVelocity() == Vector2::Zero
		&& m_pre_state != PlayerState::Ready
		&& m_pre_state != PlayerState::Return
		&& m_pre_state != PlayerState::Win
		&& m_pre_state != PlayerState::Damaged
		&& m_pre_state != PlayerState::Attack
		&& m_pre_state != PlayerState::Walk)
	{
		m_current_state = PlayerState::Idle;
	}

	//????
	if (KEY_UP(Key::KEY_ARROW_RIGHT) || KEY_UP(Key::KEY_ARROW_LEFT))
	{
		if (m_pre_state == PlayerState::Walk
			|| m_pre_state & PlayerState::Jump)
		{
			m_current_state &= ~PlayerState::Walk;
			is_walk_run = false;
		}
	}

	//????
	if (KEY_PRESS(Key::KEY_ARROW_DOWN))
	{
		m_current_state = PlayerState::Crouch;
	}

	if (is_dash)
	{
		m_current_state = PlayerState::Dash;

		//???? ?????????? ???????? ???????? ????
		if (m_side_state == SideState::Right)
			m_p_rigidbody2D->AddForce(Vector2(m_dash_speed, 0.0f));

		//?????? ???????? ????
		else
			m_p_rigidbody2D->AddForce(Vector2(-m_dash_speed, 0.0f));
	}

	//????
	if (is_jump)
	{
		m_current_state |= PlayerState::Jump;
		is_walk_run = false;
	}

	//????
	if (KEY_DOWN(Key::KEY_CONTROL))
	{
		if ((m_pre_state & PlayerState::Jump)
			|| (m_pre_state & PlayerState::Crouch)
			|| (m_pre_state & PlayerState::Slide))
		{
			m_current_state |= PlayerState::Attack;
		}

		else
		{
			//???? ?????? ?????????? ????
			if (m_pre_state == PlayerState::Attack)
			{
				if (m_attack_count == 3)
					return;

				else
				{
					//???? ???????????? ?????????? ??
					if (m_p_animator2D->GetCurrentAnimation()->GetIsFinished())
					{
						++m_attack_count;
						m_next_attck_time_limit = NEXT_ATTACK_TIME_LIMIT;
						return;
					}
				}
			}

			else
			{
				++m_attack_count;
				m_next_attck_time_limit = NEXT_ATTACK_TIME_LIMIT;
				m_current_state = PlayerState::Attack;
				is_walk_run = false;
			}
		}
	}
}

void Z_Script::Update_Animation()
{
	if (m_is_win)
	{
		m_is_ready = false;
		m_current_state = PlayerState::Win;
	}

	//Win
	if (m_current_state == PlayerState::Win)
	{
		SetCurrentAnimation("Z_Win");
	}

	//Damaged
	else if (m_current_state == PlayerState::Damaged)
	{
		SetCurrentAnimation("Z_Damaged");
	}

	//Attack
	else if (m_current_state == PlayerState::Attack)
	{
		if (m_attack_count != 0)
			SetCurrentAnimation(m_player_state_detail_map[m_current_state][m_attack_count - 1]);

		//???? ???????????? ?????? ????
		if (m_p_animator2D->GetCurrentAnimation()->GetIsFinished())
		{
			if (m_attack_count == 3)
			{
				m_attack_count = 0;
				SetCurrentAnimation("Z_Attack_End");
			}

			else
			{
				if (m_next_attck_time_limit <= 0.0f)
				{
					m_attack_count = 0;
					SetCurrentAnimation("Z_Attack_End");
				}
			}
		}
		m_next_attck_time_limit -= DELTA_TIME_F;
	}

	//Walk
	else if (m_current_state == PlayerState::Walk)
	{
		if (!is_walk_run)
			SetCurrentAnimation("Z_Walk_Begin");
	}

	//Idle
	else if (m_current_state == PlayerState::Idle)
	{
		SetCurrentAnimation("Z_Idle", true);
	}

	//Dash
	if (m_current_state & PlayerState::Dash)
	{
		if (!is_dash_run)
			SetCurrentAnimation("Z_Dash_Begin");
	}

	//Jump
	if (m_current_state & PlayerState::Jump)
	{
		if (m_current_state & PlayerState::Attack)
		{
			if (!m_p_animator2D->GetCurrentAnimation()->GetResourceName()._Equal("Z_Jump_Attack"))
				SetCurrentAnimation("Z_Jump_Attack");

			else
			{
				if (m_p_animator2D->GetCurrentAnimation()->GetIsFinished())
					m_p_animator2D->Play();
			}

			m_current_state &= ~PlayerState::Attack;
		}

		else
		{
			if (!is_jump_run)
			{
				if (m_p_animator2D->GetCurrentAnimation()->GetResourceName()._Equal("Z_Jump_Run"))
				{
					SetCurrentAnimation("Z_Fall_Begin");
					return;
				}

				if (m_p_animator2D->GetCurrentAnimation()->GetResourceName()._Equal("Z_Fall_End"))
				{
					m_current_state &= ~PlayerState::Jump;
					return;
				}

				if (is_jump && m_jump_count == 0)
				{
					SetCurrentAnimation("Z_Jump_Begin");
					++m_jump_count;
				}
			}
		}
	}

	//Slide
	if (m_current_state & PlayerState::Slide)
	{

	}

	//Crouch
	if (m_current_state & PlayerState::Crouch)
	{
		SetCurrentAnimation("Z_Crouch");
	}
}


void Z_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Hp", ScriptParamType::Int, reinterpret_cast<void*>(&m_hp), 100.0f));
	AddScriptParamData(ScriptParamStruct("Run Speed", ScriptParamType::Float, reinterpret_cast<void*>(&m_run_speed), 100.0f));
	AddScriptParamData(ScriptParamStruct("Jump Speed", ScriptParamType::Float, reinterpret_cast<void*>(&m_jump_speed), 100.0f));
	AddScriptParamData(ScriptParamStruct("Dash Speed", ScriptParamType::Float, reinterpret_cast<void*>(&m_dash_speed), 100.0f));

	AddScriptParamData(ScriptParamStruct("Current State", ScriptParamType::Int, reinterpret_cast<void*>(&m_current_state), 100.f));
}

void Z_Script::InitializePlayerStateDetailMap()
{
	std::vector<std::string> player_state_detail_vector;

	//Attack
	player_state_detail_vector.emplace_back("Z_Attack_1");
	player_state_detail_vector.emplace_back("Z_Attack_2");
	player_state_detail_vector.emplace_back("Z_Attack_3");
	m_player_state_detail_map.insert(std::make_pair(PlayerState::Attack, player_state_detail_vector));

	player_state_detail_vector.clear();
	player_state_detail_vector.shrink_to_fit();

	//Jump
	player_state_detail_vector.emplace_back("Z_Jump_Begin");
	player_state_detail_vector.emplace_back("Z_Jump_Run");
	player_state_detail_vector.emplace_back("Z_Fall_Begin");
	player_state_detail_vector.emplace_back("Z_Fall_Run");
	player_state_detail_vector.emplace_back("Z_Fall_End");
	m_player_state_detail_map.insert(std::make_pair(PlayerState::Jump, player_state_detail_vector));

	player_state_detail_vector.clear();
	player_state_detail_vector.shrink_to_fit();

	//Walk
	player_state_detail_vector.emplace_back("Z_Walk_Begin");
	player_state_detail_vector.emplace_back("Z_Walk_Run");
	m_player_state_detail_map.insert(std::make_pair(PlayerState::Walk, player_state_detail_vector));

	player_state_detail_vector.clear();
	player_state_detail_vector.shrink_to_fit();

	//Dash
	player_state_detail_vector.emplace_back("Z_Dash_Begin");
	player_state_detail_vector.emplace_back("Z_Dash_Run");
	player_state_detail_vector.emplace_back("Z_Dash_End");
	m_player_state_detail_map.insert(std::make_pair(PlayerState::Dash, player_state_detail_vector));

	player_state_detail_vector.clear();
	player_state_detail_vector.shrink_to_fit();

	//Slide
	player_state_detail_vector.emplace_back("Z_Slide_Begin");
	player_state_detail_vector.emplace_back("Z_Slide_Run");
	player_state_detail_vector.emplace_back("Z_Slide_End");
	m_player_state_detail_map.insert(std::make_pair(PlayerState::Slide, player_state_detail_vector));

	player_state_detail_vector.clear();
	player_state_detail_vector.shrink_to_fit();
}

void Z_Script::AddAnimationEvent()
{
	auto animation_map = m_p_animator2D->GetAnimationMap();

	//Ready
	animation_map["Z_Ready"]->SetAnimationEvent(21, std::bind(&Z_Script::TriggerReadyToIdleState, this));

	//Attack
	animation_map["Z_Attack_1"]->SetAnimationEvent(3, std::bind(&Z_Script::SetAttack1HitBox, this));
	animation_map["Z_Attack_1"]->SetAnimationEvent(6, std::bind(&Z_Script::SetEndAttackHitBox, this));
	animation_map["Z_Attack_2"]->SetAnimationEvent(2, std::bind(&Z_Script::SetAttack2HitBox, this));
	animation_map["Z_Attack_2"]->SetAnimationEvent(4, std::bind(&Z_Script::SetEndAttackHitBox, this));
	animation_map["Z_Attack_3"]->SetAnimationEvent(3, std::bind(&Z_Script::SetAttack3HitBox, this));
	animation_map["Z_Attack_3"]->SetAnimationEvent(7, std::bind(&Z_Script::SetEndAttackHitBox, this));
	animation_map["Z_Attack_End"]->SetAnimationEvent(5, std::bind(&Z_Script::TriggerIdleState, this));

	animation_map["Z_Jump_Attack"]->SetAnimationEvent(3, std::bind(&Z_Script::SetJumpAttackHitBox, this));
	animation_map["Z_Jump_Attack"]->SetAnimationEvent(5, std::bind(&Z_Script::SetEndAttackHitBox, this));

	animation_map["Z_Crouch_Attack"]->SetAnimationEvent(3, std::bind(&Z_Script::SetCrouchAttackHitBox, this));
	animation_map["Z_Crouch_Attack"]->SetAnimationEvent(6, std::bind(&Z_Script::SetEndAttackHitBox, this));

	animation_map["Z_Slide_Attack"]->SetAnimationEvent(2, std::bind(&Z_Script::SetSlideAttackHitBox, this));
	animation_map["Z_Slide_Attack"]->SetAnimationEvent(5, std::bind(&Z_Script::SetEndAttackHitBox, this));

	//Jump
	animation_map["Z_Jump_Begin"]->SetAnimationEvent(1, std::bind(&Z_Script::TriggerJumpRunState, this));
	animation_map["Z_Fall_Begin"]->SetAnimationEvent(6, std::bind(&Z_Script::TriggerFallRunState, this));
	animation_map["Z_Fall_End"]->SetAnimationEvent(7, std::bind(&Z_Script::TriggerIdleState, this));

	//Dash
	animation_map["Z_Dash_Begin"]->SetAnimationEvent(3, std::bind(&Z_Script::TriggerDashRunState, this));
	animation_map["Z_Dash_End"]->SetAnimationEvent(7, std::bind(&Z_Script::TriggerIdleState, this));

	//Walk
	animation_map["Z_Walk_Begin"]->SetAnimationEvent(6, std::bind(&Z_Script::TriggerWalkRunState, this));

	//Win
	animation_map["Z_Win"]->SetAnimationEvent(32, std::bind(&Z_Script::TriggerWinToReturn, this));
	
	//Return
	animation_map["Z_Return"]->SetAnimationEvent(11, m_p_win_event_func);
}

void Z_Script::SetAttack1HitBox()
{
	m_p_attack_hit_box->SetIsActive(true);

	m_p_attack_hit_box->SetOffsetPosition(Vector2(0.35f, -0.1f));
	m_p_attack_hit_box->SetOffsetScale(Vector2(0.45f, 0.5f));
}

void Z_Script::SetAttack2HitBox()
{
	m_p_attack_hit_box->SetIsActive(true);

	m_p_attack_hit_box->SetOffsetPosition(Vector2(0.35f, -0.1f));
	m_p_attack_hit_box->SetOffsetScale(Vector2(0.45f, 0.2f));
}

void Z_Script::SetAttack3HitBox()
{
	m_p_attack_hit_box->SetIsActive(true);

	m_p_attack_hit_box->SetOffsetPosition(Vector2(0.5f, -0.1f));
	m_p_attack_hit_box->SetOffsetScale(Vector2(0.7f, 0.5f));
}

void Z_Script::SetJumpAttackHitBox()
{
	m_p_attack_hit_box->SetIsActive(true);

	m_p_attack_hit_box->SetOffsetPosition(Vector2(0.4f, 0.0f));
	m_p_attack_hit_box->SetOffsetScale(Vector2(0.5f, 0.5f));
}

void Z_Script::SetCrouchAttackHitBox()
{
	m_p_attack_hit_box->SetIsActive(true);

	m_p_attack_hit_box->SetOffsetPosition(Vector2(0.35f, -0.1f));
	m_p_attack_hit_box->SetOffsetScale(Vector2(0.45f, 0.3f));
}

void Z_Script::SetSlideAttackHitBox()
{
	m_p_attack_hit_box->SetIsActive(true);

	m_p_attack_hit_box->SetOffsetPosition(Vector2(-0.4f, 0.0f));
	m_p_attack_hit_box->SetOffsetScale(Vector2(0.5f, 0.5f));
}

void Z_Script::SetEndAttackHitBox()
{
	m_p_attack_hit_box->SetOffsetPosition(Vector2::Zero);
	m_p_attack_hit_box->SetOffsetScale(Vector2::Zero);
}

void Z_Script::TriggerReadyToIdleState()
{
	m_is_ready = true;
	SetCurrentAnimation("Z_Idle", true);
	m_current_state = PlayerState::Idle;
}

void Z_Script::TriggerIdleState()
{
	SetCurrentAnimation("Z_Idle", true);
	m_current_state = PlayerState::Idle;
}

void Z_Script::TriggerWalkRunState()
{
	SetCurrentAnimation("Z_Walk_Run", true);
	is_walk_run = true;
}

void Z_Script::TriggerDashRunState()
{
	SetCurrentAnimation("Z_Dash_Run");
	is_dash_run = true;
}

void Z_Script::TriggerDashEndState()
{
	SetCurrentAnimation("Z_Dash_End");
	is_dash_run = false;
	is_dash = false;
}

void Z_Script::TriggerJumpRunState()
{
	SetCurrentAnimation("Z_Jump_Run", true);
	is_jump_run = true;
}

void Z_Script::TriggerFallRunState()
{
	SetCurrentAnimation("Z_Fall_Run", true);
}

void Z_Script::TriggerWinToReturn()
{
	SetCurrentAnimation("Z_Return");
	m_current_state = PlayerState::Return;

	LOG_INFO_F("?????????? ?????? ???????? ??????????.");
}

void Z_Script::OnCollisionEnter(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag()._Equal("Enemy"))
	{
		if (!is_hit)
		{
			is_hit = true;
			m_hp -= 5;

			LOG_INFO_F("?????????? %d?? ???????? ??????????.", 5);
		}
		m_current_state = PlayerState::Damaged;
	}

	if (p_other_game_object->GetGameObjectTag()._Equal("Colonel Attack1"))
	{
		if (!is_hit)
		{
			is_hit = true;
			m_hp -= 10;

			LOG_INFO_F("?????????? %d?? ???????? ??????????.", 10);
		}
		m_current_state = PlayerState::Damaged;
	}

	if (p_other_game_object->GetGameObjectTag()._Equal("Colonel Attack2"))
	{
		if (!is_hit)
		{
			is_hit = true;
			m_hp -= 20;

			LOG_INFO_F("?????????? %d?? ???????? ??????????.", 20);
		}

		if (!m_is_dark)
		{
			m_is_dark = true;
			p_dark_game_object->SetIsActive(true);

			LOG_INFO_F("?????????? %s ?????? ???? ???? ?????? ????????.", p_other_game_object->GetGameObjectTag().c_str());
		}
		m_current_state = PlayerState::Damaged;
	}

	if (p_other_game_object->GetGameObjectTag()._Equal("Colonel Attack3"))
	{
		if (!is_hit)
		{
			is_hit = true;
			m_hp -= 20;

			LOG_INFO_F("?????????? %d?? ???????? ??????????.", 20);
		}

		if (!m_is_distortion)
		{
			m_is_distortion = true;
			p_distortion_game_object->SetIsActive(true);

			LOG_INFO_F("?????????? %s ?????? ???? ???? ?????? ????????.", p_other_game_object->GetGameObjectTag().c_str());
		}
		m_current_state = PlayerState::Damaged;
	}

	if (m_hp <= 0)
	{
		m_hp = 0;
	}
}

void Z_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script

	GameObjectController::SaveToScene(p_file); //GameObjectController
}

void Z_Script::LoadFromScene(FILE* p_file)
{
	GameObjectController::LoadFromScene(p_file); //GameObjectController
}
