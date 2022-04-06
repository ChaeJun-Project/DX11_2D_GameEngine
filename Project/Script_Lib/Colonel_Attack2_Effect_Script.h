#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "ProjectileController.h"

class Collider2D;

class Colonel_Attack2_Effect_Script final : public Script, public ProjectileController
{
public:
	Colonel_Attack2_Effect_Script();
	explicit Colonel_Attack2_Effect_Script(const Colonel_Attack2_Effect_Script& origin);
	virtual ~Colonel_Attack2_Effect_Script();

public:
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	void RegisterScriptParamData() override;

private:
    void AddAnimationEvent();

	void UpdateCollider2DSize();

public:
	void OnCollisionEnter(GameObject* p_other_game_object) override;
	
private:
	void SaveToScene(FILE * p_file) override;
	void LoadFromScene(FILE * p_file) override;

public:
	CLONE(Colonel_Attack2_Effect_Script);

private:
	Collider2D* m_p_collider2D = nullptr;

	UINT m_call_count = 0;
};


