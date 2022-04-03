#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "GameObjectController.h"
#include "ColonelState_Enum.h"

class Transform;
class Animator2D;

class Selector;

class Z_Script;

class Colonel_Script final : public Script, public GameObjectController
{
public:
	Colonel_Script();
	explicit Colonel_Script(const Colonel_Script& origin);
	virtual ~Colonel_Script();

	void Start() override;
	void Update() override;

private:
	void RegisterScriptParamData() override;
	void MakeBehaviorTree();

	//<summary>
	//Animation의 특정 Frame에 이벤트를 추가할 필요가 있는 경우 추가
	//</summary>
private:
	void AddAnimationEvent();

	void TriggerStartToIdleState();
	void TriggerIdleState();

	void DisableCollider2D();
	void EnableCollider2D();

public:
	void OnCollisionEnter(GameObject* other_game_object) override;
	void OnCollisionStay(GameObject* other_game_object) override;
	void OnCollisionExit(GameObject* other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Colonel_Script);

private:
	Z_Script* m_p_player_script = nullptr;
	Collider2D* m_p_collider2D = nullptr;

	ColonelState m_current_state = ColonelState::Start;
	ColonelState m_pre_state;

	bool m_is_ready = false;

	//Prefab
	std::shared_ptr<Prefab> p_attack1_effect = nullptr;
	std::shared_ptr<Prefab> p_attack2_effect = nullptr;
	std::shared_ptr<Prefab> p_attack3_effect = nullptr;

	//Hit
	bool is_hit = false;
	float hit_delay = 1.0f;

	//Behavior Tree Root
	Selector* m_p_behavior_tree_root = nullptr;

	friend class CheckPlayerDeadNode;
	friend class IdleNode;
	friend class AttackNode;
	friend class GuardNode;
	friend class StealthNode;
	friend class DieNode;
};

