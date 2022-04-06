#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class GameObject;

class Ready_Script;
class Camera_Script;

class GameManager_Script final : public Script
{
public:
	GameManager_Script();
	explicit GameManager_Script(const GameManager_Script& origin);
	virtual ~GameManager_Script();

	void Start() override;
	void Update() override;

private:
	void RegisterScriptParamData() override;

private:
    void SetUI();
	void SetStageEvent();
	void SetStageLockedWall();

	void CreatePlayer();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(GameManager_Script);

private:
    //UI
	GameObject* m_p_ready_ui = nullptr;
	Ready_Script* m_p_ready_script = nullptr;
	GameObject* m_p_warning_ui = nullptr;
	GameObject* m_p_player_hp_ui = nullptr;
	GameObject* m_p_boss_hp_ui = nullptr;

	//Stage Events
	GameObject* m_p_stage_event_1 = nullptr;
	GameObject* m_p_stage_event_2 = nullptr;

	//Stage Locked Walls
	GameObject* m_p_locked_wall_1 = nullptr;
	GameObject* m_p_locked_wall_2 = nullptr;
	GameObject* m_p_locked_wall_3 = nullptr;

	//Camera Script
	Camera_Script* p_camera_script = nullptr;

	//Prefab
	std::shared_ptr<Prefab> m_p_z_prefab = nullptr;
	std::shared_ptr<Prefab> m_p_colonel_prefab = nullptr;

	//GameObject
	GameObject* m_p_z_game_object = nullptr;
	GameObject* m_p_colonel_game_object = nullptr;

	bool m_ready_to_play = false;
};

