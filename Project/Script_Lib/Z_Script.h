#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "PlayerController.h"

class Animator2D;

class Texture;
class Prefab;

class Z_Script final : public Script, public PlayerController
{
public:
	Z_Script();
	explicit Z_Script(const Z_Script& origin);
	virtual ~Z_Script();

	void Initialize() override;
	void Start() override;
	void Update() override;
	
public:
	void OnCollisionEnter(GameObject* other_game_object) override;
	void OnCollisionStay(GameObject* other_game_object) override;
	void OnCollisionExit(GameObject* other_game_object) override;

private:
	void RegisterScriptParamData() override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Z_Script);

private: 

    int      test1;
    Vector2  test2;
	Vector3  test3;
	Vector4  test4;

	Texture* p_texture;
	Prefab* p_prefab;

	Animator2D* m_p_animator = nullptr;
	AnimationState m_current_state = AnimationState::Idle;
};