#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>
class GameLogic_Script : public Script
{
public:
	GameLogic_Script();
	~GameLogic_Script();

	void Initialize() override;

	void Update() override;

public:
	virtual void OnCollisionEnter(GameObject* other_game_object) override;
	virtual void OnCollisionExit(GameObject* other_game_object) override;
	virtual void OnCollision(GameObject* other_game_object) override;

public:
	CLONE(GameLogic_Script);

private:
	float m_accumulate_time = 0.0f;

	std::shared_ptr<Prefab> m_p_walkcannon_prefab = nullptr;
};

