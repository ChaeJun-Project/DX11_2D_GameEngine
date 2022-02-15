#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "PlayerController.h"

class Animator2D;

class Z_Script final : public Script, public PlayerController
{
public:
	Z_Script();
	explicit Z_Script(const Z_Script& origin);
	~Z_Script();

	void Start() override;
	void Update() override;
	
public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

public:
	CLONE(Z_Script);

private:
    float m_speed = 200.f;

	Animator2D* m_p_animator = nullptr;

	AnimationState m_current_state = AnimationState::Idle;
};

