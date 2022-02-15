#pragma once

//TODO 상태를 비트값으로 대체하는 방법 생각하기
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

class PlayerController
{
public:
	PlayerController() = default;
	virtual ~PlayerController() = default;

private:

};
