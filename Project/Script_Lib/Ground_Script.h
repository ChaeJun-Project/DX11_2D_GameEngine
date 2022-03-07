#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>

class Ground_Script final : public Script
{
public:
	Ground_Script();
	explicit Ground_Script(const Ground_Script& origin);
	virtual ~Ground_Script();

public:
	void OnCollisionEnter(GameObject* other_game_object) override;
	void OnCollisionStay(GameObject* other_game_object) override;
	void OnCollisionExit(GameObject* other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override {}

public:
	CLONE(Ground_Script);

private:
    bool m_is_ground = false;
    float m_player_position_y = 0.0f;

};

