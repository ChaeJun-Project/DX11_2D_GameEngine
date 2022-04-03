#include "stdafx.h"
#include "Colonel_Script.h"

#include "Colonel_Behavior_Node.h"

#include "Z_Script.h"

#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>
#include <DX11_2D_GameEngine_Lib/Prefab.h>

#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
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
}

Colonel_Script::~Colonel_Script()
{
	m_p_player_script = nullptr;
	m_p_collider2D = nullptr;

	p_attack1_effect.reset();
	p_attack2_effect.reset();
	p_attack3_effect.reset();

	SAFE_DELETE(m_p_behavior_tree_root);
}

void Colonel_Script::Start()
{
	//Set Script Data
	/*p_attack1_effect = std::get<std::shared_ptr<Prefab>>(m_script_param_vector[2].m_p_param_data);
	p_attack2_effect = std::get<std::shared_ptr<Prefab>>(m_script_param_vector[3].m_p_param_data);
	p_attack3_effect = std::get<std::shared_ptr<Prefab>>(m_script_param_vector[4].m_p_param_data);*/
	
	GameObjectController::m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	GameObjectController::m_p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();

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
			hit_delay = 2.0f;
			is_hit = false;
		}
	}
}

void Colonel_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Hp", ScriptParamType::Int, reinterpret_cast<void*>(&m_hp), 100.0f));
	AddScriptParamData(ScriptParamStruct("Current State", ScriptParamType::Int, reinterpret_cast<void*>(&m_current_state), 100.f));
	
	AddScriptParamData(ScriptParamStruct("Attack1 Effect", ScriptParamType::Prefab, &p_attack1_effect, 100.f));
	AddScriptParamData(ScriptParamStruct("Attack2 Effect", ScriptParamType::Prefab, &p_attack2_effect, 100.f));
	AddScriptParamData(ScriptParamStruct("Attack3 Effect", ScriptParamType::Prefab, &p_attack3_effect, 100.f));
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

void Colonel_Script::AddAnimationEvent()
{
	auto animation_map = m_p_animator2D->GetAnimationMap();

	//Start
	animation_map["Colonel_Start"]->SetAnimationEvent(9, std::bind(&Colonel_Script::TriggerStartToIdleState, this));

	//Attack
	animation_map["Colonel_Attack_1"]->SetAnimationEvent(4, std::bind(&Colonel_Script::TriggerIdleState, this));
	animation_map["Colonel_Attack_2"]->SetAnimationEvent(5, std::bind(&Colonel_Script::TriggerIdleState, this));
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

void Colonel_Script::OnCollisionEnter(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "PlayerAttack")
	{
		if (m_pre_state == ColonelState::Guard)
			return;

		if (!is_hit)
		{
			is_hit = true;
			m_hp -= 20;
			if (m_hp <= 0)
				m_hp = 0;
		}
	}
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

	//Attack Effect
	fprintf_s(p_file, "[Attack1 Effect]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(p_attack1_effect, p_file);

	fprintf_s(p_file, "[Attack2 Effect]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(p_attack2_effect, p_file);

	fprintf_s(p_file, "[Attack3 Effect]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(p_attack3_effect, p_file);

}

void Colonel_Script::LoadFromScene(FILE* p_file)
{
	GameObjectController::LoadFromScene(p_file); //GameObjectController

	char char_buffer[256] = { 0 };

	//Attack Effect
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Attack1 Effect]
	RESOURCE_MANAGER->LoadResource<Prefab>(p_attack1_effect, p_file);

	FILE_MANAGER->FScanf(char_buffer, p_file); //[Attack2 Effect]
	RESOURCE_MANAGER->LoadResource<Prefab>(p_attack2_effect, p_file);

	FILE_MANAGER->FScanf(char_buffer, p_file); //[Attack3 Effect]
	RESOURCE_MANAGER->LoadResource<Prefab>(p_attack3_effect, p_file);
}
