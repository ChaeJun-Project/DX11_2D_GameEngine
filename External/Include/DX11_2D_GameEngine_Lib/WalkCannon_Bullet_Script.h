#pragma once
#include "Script.h"

class WalkCannon_Bullet_Script : public Script
{
public:
	WalkCannon_Bullet_Script();
	~WalkCannon_Bullet_Script();

	void Initialize() override;

	void Update() override;

	void Render();

public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

public:
	CLONE(WalkCannon_Bullet_Script);

private:
	float m_speed = 150.f;

	Vector3 m_fire_direction = Vector3::Zero;
};

