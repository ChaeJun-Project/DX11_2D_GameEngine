#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "PlayerController.h"

class Animator2D;

class X_Script final : public Script, public PlayerController
{
public:
	X_Script();
	explicit X_Script(const X_Script& origin);
	~X_Script();

	void Start() override;
	void Update() override;

public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

public:
	CLONE(X_Script);

private:
	float m_speed = 200.f;

	Animator2D* m_p_animator = nullptr;

	AnimationState m_current_state = AnimationState::Idle;
};

