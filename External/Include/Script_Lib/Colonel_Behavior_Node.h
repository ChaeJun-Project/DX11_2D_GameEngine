#pragma once

#include "stdafx.h" 
#include "BehaviourTree.h"

class Colonel_Script;

//=================
//Check Player Dead
//==================
class CheckPlayerDeadNode final : public Node
{
public:
	CheckPlayerDeadNode(Colonel_Script* p_colonel_script);
	~CheckPlayerDeadNode();

public:
	const bool Update() override;

private:
	Colonel_Script* m_p_colonel_script = nullptr;
};

//=================
//Die
//=================
class DieNode final : public Node
{
public:
	DieNode(Colonel_Script* p_colonel_script);
	~DieNode();

public:
	const bool Update() override;

private:
	Colonel_Script* m_p_colonel_script = nullptr;
};

//=================
//Attack
//=================
class AttackDecoratorNode final : public Decorator
{
public:
	AttackDecoratorNode() = default;
	~AttackDecoratorNode() = default;

public:
    void Tick() override;
	const bool Update() override;

private:
	void CheckAttackCooltime();

private:
	float m_attack_cooltime = 6.0f;
	float m_accumulate_time = 0.0f;

	bool m_able_attack = true;
};

class AttackNode final : public Node
{
public:
	AttackNode(Colonel_Script* p_colonel_script);
	~AttackNode();

public:
	const bool Update() override;

private:
	const int CreateRandomIndex();

private:
	Colonel_Script* m_p_colonel_script = nullptr;

	std::vector<std::string> m_attack_vector;

};

//=================
//Stealth
//=================
class StealthDecoratorNode final : public Decorator
{
public:
	StealthDecoratorNode() = default;
	~StealthDecoratorNode() = default;

public:
	void Tick() override;
	const bool Update() override;

private:
	void CheckStealthCooltime();

private:
	float m_stealth_cooltime = 4.0f;
	float m_accumulate_time = 0.0f;

	bool m_able_stealth = false;
};

class StealthNode final : public Node
{
public:
	StealthNode(Colonel_Script* p_colonel_script);
	~StealthNode();

public:
    void Tick() override;
	const bool Update() override;

private:
	void CheckStealthDuration();

private:
	Colonel_Script* m_p_colonel_script = nullptr;

	std::vector<std::string> m_stealth_vector;

	Vector3 m_teleport_position = Vector3::Zero;
	float m_stealth_duration = 2.0f;
	bool m_is_stealth_end = false;

	float m_accumulate_time = 0.0f;
};

//=================
//Guard
//=================
class GuardDecoratorNode final : public Decorator
{
public:
	GuardDecoratorNode() = default;
	~GuardDecoratorNode() = default;

public:
	void Tick() override;
	const bool Update() override;

private:
	void CheckGuardCooltime();

private:
	float m_guard_cooltime = 10.0f;
	float m_accumulate_time = 0.0f;

	bool m_able_guard = false;
};

class GuardNode final : public Node
{
public:
	GuardNode(Colonel_Script* p_colonel_script);
	~GuardNode();

public:
    void Tick() override;
	const bool Update() override;

private:
	void CheckGuardDuration();

private:
	Colonel_Script* m_p_colonel_script = nullptr;

	float m_guard_duration = 2.0f;
	bool m_is_guard_end = false;

	float m_accumulate_time = 0.0f;
};

//=================
//Idle
//=================
class IdleNode final : public Node
{
public:
	IdleNode(Colonel_Script* p_colonel_script);
	~IdleNode();

public:
	const bool Update() override;

private:
	Colonel_Script* m_p_colonel_script = nullptr;
};
