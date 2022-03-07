#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>

class Wall_Script final : public Script
{
public:
	Wall_Script();
	explicit Wall_Script(const Wall_Script& origin);
	virtual ~Wall_Script();

public:
	void OnCollisionEnter(GameObject* other_game_object) override;
	void OnCollisionStay(GameObject* other_game_object) override;
	void OnCollisionExit(GameObject* other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override {}

public:
	CLONE(Wall_Script);

private:
    Vector3 m_position = Vector3::Zero;
	bool m_is_right = false;
	bool m_is_wall = false;
	float m_player_position_x = 0.0f;

};