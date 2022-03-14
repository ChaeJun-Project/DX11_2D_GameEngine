#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "GameObjectController.h"
#include "PlayerState_Enum.h"

class Transform;
class Animator2D;
class RigidBody2D;
class Collider2D;

class Z_Script final : public Script, public GameObjectController
{
public:
	Z_Script();
	explicit Z_Script(const Z_Script& origin);
	virtual ~Z_Script();

	void Start() override;
	void Update() override;

private:
    void Update_Move() override;
	void Update_State() override;
	void Update_Animation() override;

private:
	void RegisterScriptParamData() override;
	void InitializePlayerStateDetailMap();

	//<summary>
	//Animation의 특정 Frame에 이벤트를 추가할 필요가 있는 경우 추가
	//</summary>
private:
	void AddAnimationEvent();

	//Attack
	void SetAttack1HitBox();
	void SetAttack2HitBox();
	void SetAttack3HitBox();

	void SetJumpAttackHitBox();
	void SetCrouchAttackHitBox();
	void SetSlideAttackHitBox();

	void SetEndAttackHitBox();

	//Trigger
	void TriggerIdleState();

	void TriggerWalkRunState();

	void TriggerDashRunState();
	void TriggerDashEndState();

	void TriggerJumpRunState();

	void TriggerFallRunState();

public:
	void OnCollisionEnter(GameObject* other_game_object) override;
	void OnCollisionStay(GameObject* other_game_object) override;
	void OnCollisionExit(GameObject* other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Z_Script);

private: 
    Transform* m_p_transform = nullptr;
	RigidBody2D* m_p_rigidbody2D = nullptr;

	GameObject* m_p_attack_hit = nullptr;
	Collider2D* m_p_attack_hit_box = nullptr;

	UINT m_current_state = PlayerState::Ready;
	UINT m_pre_state;

	std::map<UINT, std::vector<std::string>> m_player_state_detail_map;

//Attack
#define NEXT_ATTACK_TIME_LIMIT 0.5f
#define MAX_ATTACK_COUNT 3

//Jump
#define MAX_JUMP_COUNT 1

	//Attack
	UINT m_attack_count = 0;
	float m_next_attck_time_limit = NEXT_ATTACK_TIME_LIMIT;

	//Walk
	bool is_walk_run = false;
	float m_press_run = 0.0f;

	//Jump
	UINT m_jump_count = 0;
	bool is_jump = false;
	bool is_jump_run = false;

	//Dash
	bool is_dash = false;
	bool is_dash_run = false;

	//Hit
	bool is_hit = false;
	float hit_delay = 2.0f;
};
