#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class Animator2D;

class WalkCannon_Bullet_Script : public Script
{
public:
	WalkCannon_Bullet_Script();
	~WalkCannon_Bullet_Script();

	void Start() override;
	void Update() override;

public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

public:
    void SetDirection(const Vector3& fire_direction);

public:
	CLONE(WalkCannon_Bullet_Script);

private:
	float m_speed = 300.f;

	Animator2D* m_p_animator = nullptr;

	Vector3 m_fire_direction = Vector3::Zero;
};

