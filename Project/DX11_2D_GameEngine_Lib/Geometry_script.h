#pragma once
#include "Script.h"

class Geometry_script : public Script
{
public:
	Geometry_script();
	~Geometry_script();

	void Initialize() override;

	void Update() override;

public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

public:
	CLONE(Geometry_script);
};

