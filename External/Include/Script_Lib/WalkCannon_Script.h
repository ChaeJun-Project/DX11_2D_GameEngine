#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class WalkCannon_Script : public Script
{
private:
	//TODO 상태를 비트값으로 대체하는 방법 생각하기
	enum class AnimationState : UINT
	{
		Idle,
		LandFireMid,
		LandFireTop,
		LandFireBot,
		Walk,
		Jump,
		JumpFireMid,
		JumpFireTop,
		JumpFireBot,
	};

public:
	WalkCannon_Script();
	explicit WalkCannon_Script(const WalkCannon_Script& origin);
	~WalkCannon_Script();

	void Start() override;
	void Update() override;

public:
	void OnCollisionEnter(GameObject* other_game_object) override;
	void OnCollisionStay(GameObject* other_game_object) override;
	void OnCollisionExit(GameObject* other_game_object) override;

private:
    void CreateBullet();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(WalkCannon_Script);

private:
	float m_speed = 100.f;

	float m_idle_range = 300.0f;

	float m_accumulate_time = 0.0f;
	float m_fire_cooltime = 2.0f;

	Vector3 m_fire_position = Vector3::Zero;
	std::shared_ptr<Prefab> m_p_bullet_prefab = nullptr;

	GameObject* m_p_player = nullptr;

	//Animator* m_p_animator = nullptr;

	AnimationState m_current_state = AnimationState::Idle;
};

