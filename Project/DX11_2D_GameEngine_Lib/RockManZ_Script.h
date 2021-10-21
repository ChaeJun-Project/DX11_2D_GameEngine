#pragma once

#include "Script.h"

class RockManZ_Script : public Script
{
private:
	//TODO ���¸� ��Ʈ������ ��ü�ϴ� ��� �����ϱ�
	enum class AnimationState : UINT
	{
		Ready,
		Return,

		Idle,

		Attack_1,
		Attack_2,
		Attack_3,
		Attack_End,

		Jump_Begin,
		Jump_Run,
		Fall_Begin,
		Fall_Run,
		Fall_End,
		Jump_Attack,

		Walk_Begin,
		Walk_Run,

		Dash_Begin,
		Dash_Run,
		Dash_End,


		Slide_Begin,
		Slide_End,
		Slide_Run,
		Slide_Attack,

		Damaged,
		Big_Damaged,

		Crouch,
		Crouch_Attack,

		Win
	};


public:
	RockManZ_Script();
	~RockManZ_Script();

	void Initialize() override;

	void Update() override;

	void Render();

public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

private:
    float m_speed = 200.f;
};

