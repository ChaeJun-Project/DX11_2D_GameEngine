#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class Geometry_Script : public Script
{
public:
	Geometry_Script();
	~Geometry_Script();

	void Start() override;
	void Update() override;

public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

public:
	CLONE(Geometry_Script);
};

