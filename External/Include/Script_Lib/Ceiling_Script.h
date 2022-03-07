#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>

class Ceiling_Script final : public Script
{
public:
	Ceiling_Script();
	explicit Ceiling_Script(const Ceiling_Script& origin);
	virtual ~Ceiling_Script();

public:
	void OnCollisionEnter(GameObject* other_game_object) override;
	void OnCollisionStay(GameObject* other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override {}

public:
	CLONE(Ceiling_Script);

private:
	bool m_is_ceiling = false;
	float m_player_position_y = 0.0f;
};
