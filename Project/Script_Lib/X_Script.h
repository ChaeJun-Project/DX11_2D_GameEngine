#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "GameObjectController.h"
#include "PlayerState_Enum.h"

class Animator2D;

class X_Script final : public Script, public GameObjectController
{
public:
	X_Script();
	explicit X_Script(const X_Script& origin);
	virtual ~X_Script();

	void Start() override;
	void Update() override;

private:
	void Update_Move() override;
	void Update_State() override;
	void Update_Animation() override;

private:
	void RegisterScriptParamData() override;

public:
	void OnCollisionEnter(GameObject* other_game_object) override;
	void OnCollisionStay(GameObject* other_game_object) override;
	void OnCollisionExit(GameObject* other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(X_Script);

private:
	Transform* m_p_transform = nullptr;
	RigidBody2D* m_p_rigidbody2D = nullptr;


};

