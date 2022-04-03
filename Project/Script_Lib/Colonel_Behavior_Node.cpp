#include "stdafx.h"
#include "Colonel_Behavior_Node.h"

#include "Colonel_Script.h"
#include "Z_Script.h"

//=============
//CheckPlayerDead
//=============
CheckPlayerDeadNode::CheckPlayerDeadNode(Colonel_Script* p_colonel_script)
{
	m_p_colonel_script = p_colonel_script;
}

CheckPlayerDeadNode::~CheckPlayerDeadNode()
{
	m_p_colonel_script = nullptr;
}

const bool CheckPlayerDeadNode::Update()
{
	auto p_player_script = m_p_colonel_script->m_p_player_script;
	auto player_hp = p_player_script->GetHp();
	if (player_hp == 0)
		return true;

	return false;
}

//=============
//Die
//=============
DieNode::DieNode(Colonel_Script* p_colonel_script)
{
	m_p_colonel_script = p_colonel_script;
}

DieNode::~DieNode()
{
	m_p_colonel_script = nullptr;
}

const bool DieNode::Update()
{
	auto hp = m_p_colonel_script->GetHp();
	if (hp == 0)
	{
		m_p_colonel_script->SetCurrentAnimation("Colonel_Die");
		m_p_colonel_script->m_current_state = ColonelState::Die;
		return true;
	}

	return false;
}

void AttackDecoratorNode::Tick()
{
	if (!m_able_attack)
		CheckAttackCooltime();
}

//=============
//Attack
//=============
//Attack Decorator
const bool AttackDecoratorNode::Update()
{
	if (!m_able_attack)
		return false;

	else
	{
		auto result = m_p_child_node->Update();
		if (result)
			m_able_attack = false;
		return result;
	}
}

void AttackDecoratorNode::CheckAttackCooltime()
{
	m_accumulate_time += DELTA_TIME_F;

	if (m_accumulate_time >= m_attack_cooltime)
	{
		m_accumulate_time = 0.0f;
		m_able_attack = true;
	}
}

//Attack Node
AttackNode::AttackNode(Colonel_Script* p_colonel_script)
{
	m_p_colonel_script = p_colonel_script;

	m_attack_vector =
	{
	  "Colonel_Attack_1",
	  "Colonel_Attack_2",
	  "Colonel_Attack_3",
	};
}

AttackNode::~AttackNode()
{
	m_p_colonel_script = nullptr;

	m_attack_vector.clear();
	m_attack_vector.shrink_to_fit();
}

const bool AttackNode::Update()
{
	if (m_p_colonel_script->m_pre_state == ColonelState::Idle)
	{
		auto random_index = CreateRandomIndex();
		m_p_colonel_script->SetCurrentAnimation(m_attack_vector[random_index]);
		m_p_colonel_script->m_current_state = ColonelState::Attack;
		return true;
	}

	else
		return false;
}

const int AttackNode::CreateRandomIndex()
{
	std::random_device random_device;
	std::mt19937_64 radom_index_gen(random_device());
	std::uniform_int_distribution<int> dis(0, 2);

	return dis(radom_index_gen);
}

//=============
//Stealth
//=============
void StealthDecoratorNode::Tick()
{
	if (!m_able_stealth)
		CheckStealthCooltime();

	if (m_p_child_node == nullptr)
		return;

	m_p_child_node->Tick();
}

const bool StealthDecoratorNode::Update()
{
	if (!m_able_stealth)
		return false;

	else
	{
		auto result = m_p_child_node->Update();
		if (result)
			m_able_stealth = false;
		return result;
	}
}

void StealthDecoratorNode::CheckStealthCooltime()
{
	m_accumulate_time += DELTA_TIME_F;

	if (m_accumulate_time >= m_stealth_cooltime)
	{
		m_accumulate_time = 0.0f;
		m_able_stealth = true;
	}
}

StealthNode::StealthNode(Colonel_Script* p_colonel_script)
{
	m_p_colonel_script = p_colonel_script;

	m_stealth_vector =
	{
	  "Colonel_Stealth_Begin",
	  "Colonel_Stealth_End",
	};
}

StealthNode::~StealthNode()
{
	m_p_colonel_script = nullptr;
}

void StealthNode::Tick()
{
	if (m_p_colonel_script->m_pre_state == ColonelState::Stealth)
	{
		CheckStealthDuration();

		if (m_is_stealth_end)
		{
			m_p_colonel_script->SetPosition(m_teleport_position);

			m_p_colonel_script->SetCurrentAnimation(m_stealth_vector[1]);
			m_is_stealth_end = false;
		}
	}
}

const bool StealthNode::Update()
{
	if (m_p_colonel_script->m_pre_state == ColonelState::Idle)
	{
		auto p_player_script = m_p_colonel_script->m_p_player_script;
		auto player_position = p_player_script->GetPosition();
		m_teleport_position = m_p_colonel_script->GetPosition();

		m_teleport_position.x = player_position.x;

		m_p_colonel_script->SetCurrentAnimation(m_stealth_vector[0]);
		m_p_colonel_script->m_current_state = ColonelState::Stealth;

		return true;
	}

	else
		return false;
}

void StealthNode::CheckStealthDuration()
{
	m_accumulate_time += DELTA_TIME_F;

	if (m_accumulate_time >= m_stealth_duration)
	{
		m_accumulate_time = 0.0f;
		m_is_stealth_end = true;
	}
}

//=============
//Guard
//=============
void GuardDecoratorNode::Tick()
{
	if (!m_able_guard)
		CheckGuardCooltime();

	if (m_p_child_node == nullptr)
		return;

	m_p_child_node->Tick();
}

const bool GuardDecoratorNode::Update()
{
	if (!m_able_guard)
		return false;

	else
	{
		auto result = m_p_child_node->Update();
		if (result)
			m_able_guard = false;
		return result;
	}
}

void GuardDecoratorNode::CheckGuardCooltime()
{
	m_accumulate_time += DELTA_TIME_F;

	if (m_accumulate_time >= m_guard_cooltime)
	{
		m_accumulate_time = 0.0f;
		m_able_guard = true;
	}
}

GuardNode::GuardNode(Colonel_Script* p_colonel_script)
{
	m_p_colonel_script = p_colonel_script;
}

GuardNode::~GuardNode()
{
	m_p_colonel_script = nullptr;
}

void GuardNode::Tick()
{
	if (m_p_colonel_script->m_pre_state == ColonelState::Guard)
	{
		CheckGuardDuration();

		if (m_is_guard_end)
		{
			m_p_colonel_script->SetCurrentAnimation("Colonel_Idle", true);
			m_p_colonel_script->m_current_state = ColonelState::Idle;
			m_p_colonel_script->m_pre_state = m_p_colonel_script->m_current_state;

			m_is_guard_end = false;
		}
	}
}

const bool GuardNode::Update()
{
	if (m_p_colonel_script->m_pre_state == ColonelState::Idle)
	{
		auto p_player_script = m_p_colonel_script->m_p_player_script;
		auto player_position = p_player_script->GetPosition();
		auto position = m_p_colonel_script->GetPosition();
		if (player_position.x >= position.x) //플레이어가 Colonel보다 오른쪽에 위치한 경우
			m_p_colonel_script->m_side_state = SideState::Right; //GameObject를 오른쪽 방향으로 회전
		else //플레이어가 Colonel보다 왼쪽에 위치한 경우
			m_p_colonel_script->m_side_state = SideState::Left; //GameObject를 오른쪽 방향으로 회전

		m_p_colonel_script->SetCurrentAnimation("Colonel_Guard", true);
		m_p_colonel_script->m_current_state = ColonelState::Guard;

		return true;
	}

	else
		return false;
}

void GuardNode::CheckGuardDuration()
{
	m_accumulate_time += DELTA_TIME_F;

	if (m_accumulate_time >= m_guard_duration)
	{
		m_accumulate_time = 0.0f;
		m_is_guard_end = true;
	}
}

//=============
//Idle
//=============
IdleNode::IdleNode(Colonel_Script* p_colonel_script)
{
	m_p_colonel_script = p_colonel_script;
}

IdleNode::~IdleNode()
{
	m_p_colonel_script = nullptr;
}

const bool IdleNode::Update()
{
	if (m_p_colonel_script->m_pre_state == ColonelState::Idle)
	{
		auto p_player_script = m_p_colonel_script->m_p_player_script;
		auto player_position = p_player_script->GetPosition();
		auto position = m_p_colonel_script->GetPosition();
		if (player_position.x >= position.x) //플레이어가 Colonel보다 오른쪽에 위치한 경우
			m_p_colonel_script->m_side_state = SideState::Right; //GameObject를 오른쪽 방향으로 회전
		else //플레이어가 Colonel보다 왼쪽에 위치한 경우
			m_p_colonel_script->m_side_state = SideState::Left; //GameObject를 오른쪽 방향으로 회전

		return true;
	}

	return false;
}