#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>

class Transform;
class Collider2D;

class Ground_Script final : public Script
{
public:
	Ground_Script();
	explicit Ground_Script(const Ground_Script& origin);
	virtual ~Ground_Script();

	void Awake() override;

private:
    void PlayerCollisionEnter(GameObject* p_player_game_object);
	void PlayerCollisionStay(GameObject* p_player_game_object);
	void PlayerCollisionExit(GameObject* p_player_game_object);

public:
	void OnCollisionEnter(GameObject* p_other_game_object) override;
	void OnCollisionStay(GameObject* p_other_game_object) override;
	void OnCollisionExit(GameObject* p_other_game_object) override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override {}

public:
	CLONE(Ground_Script);

private:
	Transform* m_p_transform = nullptr;
	Collider2D* m_p_collider2D = nullptr;

    bool m_is_ground = false;
    float m_player_position_y = 0.0f;
};

