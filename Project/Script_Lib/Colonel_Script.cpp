#include "stdafx.h"
#include "Colonel_Script.h"

#include "Colonel_Behavior_Node.h"

#include "Z_Script.h"

#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>
#include <DX11_2D_GameEngine_Lib/Prefab.h>

#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/SpriteRenderer.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>

Colonel_Script::Colonel_Script()
	:Script("Colonel_Script")
{
	RegisterScriptParamData();

	MakeBehaviorTree();

	m_pre_state = m_current_state;

	m_side_state = SideState::Left;
}

Colonel_Script::Colonel_Script(const Colonel_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	MakeBehaviorTree();

	m_hp = origin.m_hp;
	
	m_p_attack1_effect = origin.m_p_attack1_effect;
	m_p_attack2_effect = origin.m_p_attack2_effect;
	m_p_attack3_effect = origin.m_p_attack3_effect;
}

Colonel_Script::~Colonel_Script()
{
	m_p_player_script = nullptr;
	m_p_collider2D = nullptr;

	m_p_attack_1_fire_transform = nullptr;
	m_p_attack_2_fire_transform = nullptr;

	for (auto& p_attack_3_fire_transform : m_p_attack_3_fire_transform_vector)
	{
		p_attack_3_fire_transform = nullptr;
	}
	m_p_attack_3_fire_transform_vector.clear();
	m_p_attack_3_fire_transform_vector.shrink_to_fit();

	//Prefab
	m_p_attack1_effect.reset();
	m_p_attack2_effect.reset();
	m_p_attack3_effect.reset();

	SAFE_DELETE(m_p_behavior_tree_root);
}

void Colonel_Script::Start()
{
	GameObjectController::m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	GameObjectController::m_p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();

	m_p_sprite_renderer = m_p_owner_game_object->GetComponent<SpriteRenderer>();
	m_p_collider2D = m_p_owner_game_object->GetComponent<Collider2D>();

	auto p_player_game_object = SCENE_MANAGER->GetCurrentScene()->FindGameObjectWithTag("Player");

	if (p_player_game_object->GetGameObjectName()._Equal("Z"))
	{
		auto p_script = p_player_game_object->GetScript("Z_Script");
		m_p_player_script = dynamic_cast<Z_Script*>(p_script); //다운 캐스팅
	}

	else if (p_player_game_object->GetGameObjectName()._Equal("X"))
	{
		//TODO
	}

	AddAttackFireTransform();

	m_p_attack_3_prepare_effect = m_p_owner_game_object->GetChildFromIndex(3);

	AddAnimationEvent();

	SetCurrentAnimation("Colonel_Start");
}

void Colonel_Script::Update()
{
	if (m_is_ready)
	{
		m_p_behavior_tree_root->Tick();
		if (m_p_behavior_tree_root->Update())
			m_pre_state = m_current_state;
	}


	//바라보는 방향에 따라 회전
	//왼쪽
	if (m_side_state == SideState::Left)
		m_p_transform->SetRotation(Quaternion::Identity);

	//오른쪽
	else
		m_p_transform->SetRotation(Quaternion::QuaternionFromEulerAngle(Vector3(0.0f, 180.0f, 0.0f)));

	if (is_hit)
	{
		hit_delay -= DELTA_TIME_F;
		if (hit_delay <= 0.0f)
		{
			hit_delay = 1.0f;
			is_hit = false;
		}
	}
}

void Colonel_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Hp", ScriptParamType::Int, reinterpret_cast<void*>(&m_hp), 100.0f));
	AddScriptParamData(ScriptParamStruct("Current State", ScriptParamType::Int, reinterpret_cast<void*>(&m_current_state), 100.f));
	
	AddScriptParamData(ScriptParamStruct("Attack1 Effect", ScriptParamType::Prefab, &m_p_attack1_effect, 100.f));
	AddScriptParamData(ScriptParamStruct("Attack2 Effect", ScriptParamType::Prefab, &m_p_attack2_effect, 100.f));
	AddScriptParamData(ScriptParamStruct("Attack3 Effect", ScriptParamType::Prefab, &m_p_attack3_effect, 100.f));
}

void Colonel_Script::MakeBehaviorTree()
{
	m_p_behavior_tree_root = new Selector();

	auto p_check_player_dead_node = new CheckPlayerDeadNode(this);

	auto p_state_selector_node = new Selector;

	//Root
	m_p_behavior_tree_root->AddChild(p_check_player_dead_node);
	m_p_behavior_tree_root->AddChild(p_state_selector_node);

	//State Selector
	//Die
	auto p_die_node = new DieNode(this);

	//Attack
	auto p_attack_decorator_node = new AttackDecoratorNode;
	auto p_attack_node = new AttackNode(this);
	p_attack_decorator_node->AddChild(p_attack_node);

	//Stealth & Guard
	//Random Selector
	auto p_random_selector = new RandomSelector;
	//Stealth
	auto p_stealth_decorator_node = new StealthDecoratorNode;
	auto p_stealth_node = new StealthNode(this);
	p_stealth_decorator_node->AddChild(p_stealth_node);
	//Guard
	auto p_guard_decorator_node = new GuardDecoratorNode;
	auto p_guard_node = new GuardNode(this);
	p_guard_decorator_node->AddChild(p_guard_node);

	p_random_selector->AddChild(p_stealth_decorator_node);
	p_random_selector->AddChild(p_guard_decorator_node);

	//Idle
	auto p_idle_node = new IdleNode(this);

	p_state_selector_node->AddChild(p_die_node);
	p_state_selector_node->AddChild(p_attack_decorator_node);
	p_state_selector_node->AddChild(p_random_selector);
	p_state_selector_node->AddChild(p_idle_node);
}

void Colonel_Script::AddAttackFireTransform()
{
    //Attack_1
	m_p_attack_1_fire_transform = m_p_owner_game_object->GetChildFromIndex(0)->GetComponent<Transform>();

	//Attack_2
	m_p_attack_2_fire_transform = m_p_owner_game_object->GetChildFromIndex(1)->GetComponent<Transform>();

	//Attack_3
	auto p_attack_3_fire_position_game_objects = m_p_owner_game_object->GetChildFromIndex(2)->GetChilds();

	for (const auto& p_attack_3_fire_position_game_object : p_attack_3_fire_position_game_objects)
	{
		m_p_attack_3_fire_transform_vector.emplace_back(p_attack_3_fire_position_game_object->GetComponent<Transform>());
	}
}

void Colonel_Script::AddAnimationEvent()
{
	auto animation_map = m_p_animator2D->GetAnimationMap();

	//Start
	animation_map["Colonel_Start"]->SetAnimationEvent(9, std::bind(&Colonel_Script::TriggerStartToIdleState, this));

	//Attack
	animation_map["Colonel_Attack_1"]->SetAnimationEvent(2, std::bind(&Colonel_Script::CreateAttack1Effect, this));
	animation_map["Colonel_Attack_1"]->SetAnimationEvent(4, std::bind(&Colonel_Script::TriggerIdleState, this));

	animation_map["Colonel_Attack_2"]->SetAnimationEvent(3, std::bind(&Colonel_Script::CreateAttack2Effect, this));
	animation_map["Colonel_Attack_2"]->SetAnimationEvent(5, std::bind(&Colonel_Script::TriggerIdleState, this));

	animation_map["Colonel_Attack_3"]->SetAnimationEvent(0, std::bind(&Colonel_Script::ActiveAttack3PrepareEffect, this));
	animation_map["Colonel_Attack_3"]->SetAnimationEvent(4, std::bind(&Colonel_Script::DisactiveAttack3PrepareEffect, this));
	animation_map["Colonel_Attack_3"]->SetAnimationEvent(17, std::bind(&Colonel_Script::CreateAttack3Effect, this));
	animation_map["Colonel_Attack_3"]->SetAnimationEvent(17, std::bind(&Colonel_Script::TriggerIdleState, this));

	//Stealth
	animation_map["Colonel_Stealth_Begin"]->SetAnimationEvent(4, std::bind(&Colonel_Script::DisableCollider2D, this));
	animation_map["Colonel_Stealth_End"]->SetAnimationEvent(4, std::bind(&Colonel_Script::TriggerIdleState, this));
	animation_map["Colonel_Stealth_End"]->SetAnimationEvent(4, std::bind(&Colonel_Script::EnableCollider2D, this));
}

void Colonel_Script::TriggerStartToIdleState()
{
	m_is_ready = true;
	SetCurrentAnimation("Colonel_Idle", true);
	m_current_state = ColonelState::Idle;
	m_pre_state = m_current_state;
}

void Colonel_Script::TriggerIdleState()
{
	SetCurrentAnimation("Colonel_Idle", true);
	m_current_state = ColonelState::Idle;
	m_pre_state = m_current_state;
}

void Colonel_Script::DisableCollider2D()
{
	m_p_collider2D->SetIsActive(false);
	m_p_collider2D->UpdateConstantBuffer();
}

void Colonel_Script::EnableCollider2D()
{
	m_p_collider2D->SetIsActive(true);
	m_p_collider2D->UpdateConstantBuffer();
}

#include "Colonel_Attack1_Effect_Script.h"
void Colonel_Script::CreateAttack1Effect()
{
    auto attack1_start_position = Vector3::Zero;
	attack1_start_position.x = m_p_attack_1_fire_transform->GetTranslation().x;
	attack1_start_position.y = m_p_attack_1_fire_transform->GetLocalTranslation().y;
	
	auto attack1_effect = Instantiate(m_p_attack1_effect.get(), attack1_start_position);
	auto p_attack1_effect_script = dynamic_cast<Colonel_Attack1_Effect_Script*>(attack1_effect->GetScript("Colonel_Attack1_Effect_Script"));
	
	auto fire_direction = Vector3::Zero;
	switch (m_side_state)
	{
	case SideState::Left:
		fire_direction = Vector3::Left;
		p_attack1_effect_script->SetSideState(SideState::Left);
		break;
	case SideState::Right:
		fire_direction = Vector3::Right;
		p_attack1_effect_script->SetSideState(SideState::Right);
		break;
	}

	p_attack1_effect_script->SetFireDirection(fire_direction);
}

#include "Colonel_Attack2_Effect_Script.h"
void Colonel_Script::CreateAttack2Effect()
{
	auto attack2_start_position = Vector3::Zero;
	attack2_start_position.x = m_p_attack_2_fire_transform->GetTranslation().x;
	attack2_start_position.y = m_p_attack_2_fire_transform->GetLocalTranslation().y;

	auto attack2_effect = Instantiate(m_p_attack2_effect.get(), attack2_start_position);
	auto p_attack2_effect_script = dynamic_cast<Colonel_Attack2_Effect_Script*>(attack2_effect->GetScript("Colonel_Attack2_Effect_Script"));

	auto fire_direction = Vector3::Zero;
	switch (m_side_state)
	{
	case SideState::Left:
		fire_direction = Vector3::Left;
		p_attack2_effect_script->SetSideState(SideState::Left);
		break;
	case SideState::Right:
		fire_direction = Vector3::Right;
		p_attack2_effect_script->SetSideState(SideState::Right);
		break;
	}

	p_attack2_effect_script->SetFireDirection(fire_direction);
}

void Colonel_Script::ActiveAttack3PrepareEffect()
{
	m_p_attack_3_prepare_effect->SetIsActive(true);
	auto p_animator2D = m_p_attack_3_prepare_effect->GetComponent<Animator2D>();
	p_animator2D->Play();
}

void Colonel_Script::DisactiveAttack3PrepareEffect()
{
	m_p_attack_3_prepare_effect->SetIsActive(false);
	auto p_animator2D = m_p_attack_3_prepare_effect->GetComponent<Animator2D>();
	p_animator2D->Stop();
}

#include "Colonel_Attack3_Effect_Script.h"
void Colonel_Script::CreateAttack3Effect()
{
	for (const auto& p_attack_3_fire_transform : m_p_attack_3_fire_transform_vector)
	{
		auto attack3_start_position = Vector3::Zero;
		attack3_start_position.x = p_attack_3_fire_transform->GetTranslation().x;
		attack3_start_position.y = p_attack_3_fire_transform->GetLocalTranslation().y;

		auto attack3_effect = Instantiate(m_p_attack3_effect.get(), attack3_start_position);
		auto p_attack3_effect_script = dynamic_cast<Colonel_Attack3_Effect_Script*>(attack3_effect->GetScript("Colonel_Attack3_Effect_Script"));

		auto fire_direction = Vector3::Up;
		
		p_attack3_effect_script->SetFireDirection(fire_direction);
	}
}

void Colonel_Script::OnCollisionEnter(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "PlayerAttack")
	{
		if (m_pre_state == ColonelState::Guard || m_pre_state== ColonelState::Die)
			return;

		if (!is_hit)
		{
			is_hit = true;
			m_p_sprite_renderer->SetSpriteTextureColor(Vector4::Red);
			m_hp -= 20;
			if (m_hp <= 0)
				m_hp = 0;
		}
	}
}

void Colonel_Script::OnCollisionExit(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "PlayerAttack")
	{
		m_p_sprite_renderer->SetSpriteTextureColor(Vector4::White);
	}
}

void Colonel_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script

	GameObjectController::SaveToScene(p_file); //GameObjectController

	//Attack Effect
	fprintf_s(p_file, "[Attack1 Effect]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(m_p_attack1_effect, p_file);

	fprintf_s(p_file, "[Attack2 Effect]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(m_p_attack2_effect, p_file);

	fprintf_s(p_file, "[Attack3 Effect]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(m_p_attack3_effect, p_file);

}

void Colonel_Script::LoadFromScene(FILE* p_file)
{
	GameObjectController::LoadFromScene(p_file); //GameObjectController

	char char_buffer[256] = { 0 };

	//Attack Effect
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Attack1 Effect]
	RESOURCE_MANAGER->LoadResource<Prefab>(m_p_attack1_effect, p_file);

	FILE_MANAGER->FScanf(char_buffer, p_file); //[Attack2 Effect]
	RESOURCE_MANAGER->LoadResource<Prefab>(m_p_attack2_effect, p_file);

	FILE_MANAGER->FScanf(char_buffer, p_file); //[Attack3 Effect]
	RESOURCE_MANAGER->LoadResource<Prefab>(m_p_attack3_effect, p_file);
}
