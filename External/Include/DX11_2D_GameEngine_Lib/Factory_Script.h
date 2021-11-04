#pragma once
#include "Script.h"
class Factory_Script : public Script
{
public:
    Factory_Script();
    ~Factory_Script();

    void Initialize() override;

    void Update() override;

public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

public:
	CLONE(Factory_Script);

private:


};

