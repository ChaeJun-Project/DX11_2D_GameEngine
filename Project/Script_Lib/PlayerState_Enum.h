#pragma once

enum PlayerState : UINT
{
	Ready		= 1U << 0,
	Return		= 1U << 1,
	Win			= 1U << 2,
	Damaged		= 1U << 3,
	Attack		= 1U << 4,
	Walk		= 1U << 5,
	Idle		= 1U << 6,

	Dash		= 1U << 7,
	Jump		= 1U << 8,
	Slide		= 1U << 9,
	Crouch		= 1U << 10,

	Jump_Attack	  = Attack | Jump, 
	Slide_Attack  = Attack | Slide,
	Crouch_Attack = Attack | Crouch,

	Dash_Jump	  = Dash | Jump,
};

enum class AttackState : UINT
{
	Attack_1,
	Attack_2,
	Attack_3,
	Attack_End
};

enum class JumpState : UINT
{
	Jump_Begin,
	Jump_Run
};

enum class FallState : UINT
{
	Fall_Begin,
	Fall_Run,
	Fall_End,
};

enum class WalkState : UINT
{
	Walk_Begin,
	Walk_Run,
};

enum class DashState : UINT
{
	Dash_Begin,
	Dash_Run,
	Dash_End,
};

enum class SlideState : UINT
{
	Slide_Begin,
	Slide_End,
	Slide_Run,
};

//합성 애니메이션
//Jump_Attack
//Slide_Attack
//Crouch_Attack