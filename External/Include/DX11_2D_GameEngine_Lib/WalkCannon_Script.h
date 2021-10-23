#pragma once
#include "Script.h"

class WalkCannon_Script : public Script
{
private:
	//TODO ���¸� ��Ʈ������ ��ü�ϴ� ��� �����ϱ�
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
	~WalkCannon_Script();

	void Initialize() override;

	void Update() override;

	void Render();

public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

public:
	CLONE(WalkCannon_Script);

private:
	float m_speed = 100.f;
};

