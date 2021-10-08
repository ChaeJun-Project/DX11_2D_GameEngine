#pragma once
#include "IScript.h"
class RockManZ_Script : public IScript
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
	RockManZ_Script(GameObject* p_game_object);
	~RockManZ_Script();

	void Initialize();

	void Update() override;

	void Render();
};

