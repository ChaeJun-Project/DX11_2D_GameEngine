#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>

class Transform;
class Animator2D;
class Collider2D;
class AudioSource;

class Door_Script final : public Script
{
public:
	Door_Script();
	explicit Door_Script(const Door_Script& origin);
	virtual ~Door_Script();

	void Awake() override;
	void Update() override;

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
	CLONE(Door_Script);

private:
	Transform* m_p_transform = nullptr;
	Animator2D* m_p_animator2D = nullptr;
	Collider2D* m_p_collider2D = nullptr;
    bool is_open = false;
	AudioSource* m_p_audio_source = nullptr;
 
	bool m_is_right = false;
	bool m_is_wall = false;
	float m_player_position_x = 0.0f;
};

