#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class Animator2D;

class ExplosionEffect_Script final : public Script
{
public:
	ExplosionEffect_Script();
	explicit ExplosionEffect_Script(const ExplosionEffect_Script& origin);
	~ExplosionEffect_Script();

public:
	void Awake() override;
	void OnEnable() override;
	void OnDisable() override;
	void Update() override;

private:
	void SaveToScene(FILE * p_file) override;
	void LoadFromScene(FILE * p_file) override {}

public:
	CLONE(ExplosionEffect_Script);

private:
	Animator2D* m_p_animator2D = nullptr;
};

