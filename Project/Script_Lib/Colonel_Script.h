#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "GameObjectController.h"
#include "ColonelState_Enum.h"

class Z_Script;

class Transform;
class SpriteRenderer;
class Collider2D;

class Selector;

class Colonel_Script final : public Script, public GameObjectController
{
public:
	Colonel_Script();
	explicit Colonel_Script(const Colonel_Script& origin);
	virtual ~Colonel_Script();

public:
    void Awake() override;
	void Start() override;
	void Update() override;

private:
	void RegisterScriptParamData() override;
	void MakeBehaviorTree();
	void AddAttackFireTransform();

	//<summary>
	//Animation의 특정 Frame에 이벤트를 추가할 필요가 있는 경우 추가
	//</summary>
private:
	void AddAnimationEvent();

	void TriggerStartToIdleState();
	void TriggerIdleState();

	void DisableCollider2D();
	void EnableCollider2D();

	void CreateAttack1Effect();
	void CreateAttack2Effect();

	void ActiveAttack3PrepareEffect();
	void DisactiveAttack3PrepareEffect();
	void CreateAttack3Effect();

public:
	void OnCollisionEnter(GameObject* p_other_game_object) override;
	void OnCollisionExit(GameObject* p_other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Colonel_Script);

private:
	Z_Script* m_p_player_script = nullptr;
	SpriteRenderer* m_p_sprite_renderer = nullptr;
	Collider2D* m_p_collider2D = nullptr;

	ColonelState m_current_state = ColonelState::Start;
	ColonelState m_pre_state;

	bool m_is_ready = false;

	Transform* m_p_attack_1_fire_transform = nullptr;
	Transform* m_p_attack_2_fire_transform = nullptr;
	std::vector<Transform*> m_p_attack_3_fire_transform_vector;

	//Prefab
	std::shared_ptr<Prefab> m_p_attack1_effect = nullptr;
	std::shared_ptr<Prefab> m_p_attack2_effect = nullptr;
	std::shared_ptr<Prefab> m_p_attack3_effect = nullptr;

	GameObject* m_p_attack_3_prepare_effect = nullptr;

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

