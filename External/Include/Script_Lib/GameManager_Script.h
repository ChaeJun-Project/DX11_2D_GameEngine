#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class GameObject;
class AudioSource;

class Ready_Script;
class Warning_Script;
class Hp_Script;
class Camera_Script;
class Z_Script;
class Colonel_Script;

class GameManager_Script final : public Script
{
public:
	GameManager_Script();
	explicit GameManager_Script(const GameManager_Script& origin);
	virtual ~GameManager_Script();

public:
    void Awake() override;
	void Start() override;

private:
	void RegisterScriptParamData() override;

private:
    void SetUI();
	void SetStageEvent();
	void SetStageLockedWall();
	void SetStageNormalWall();

	//UI
	//Ready
	void ReadyToPlay();

	//Stage Event
	void ReadyStage1();
	void ReadyStage2();

	void StartStage();

	void PlayerWin();

	//End Stage
	void EndStage1();
	void EndStage();
	void LoadScene();

	//GameObject
	void CreatePlayer();
	void CreateBoss();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(GameManager_Script);

private:
    //Audio
	AudioSource* m_p_audio_source = nullptr;

    //UI
	GameObject* m_p_ready_ui = nullptr;
	Ready_Script* m_p_ready_script = nullptr;

	GameObject* m_p_warning_ui = nullptr;
	Warning_Script* m_p_warning_script = nullptr;

	GameObject* m_p_player_hp_gage = nullptr;
	Hp_Script* m_p_player_hp_script = nullptr;

	GameObject* m_p_boss_hp_gage = nullptr;
	Hp_Script* m_p_boss_hp_script = nullptr;

	//Stage Events
	GameObject* m_p_stage_event_1 = nullptr;
	GameObject* m_p_stage_event_2 = nullptr;

	//Stage Locked Walls
	GameObject* m_p_locked_wall_1 = nullptr;
	GameObject* m_p_locked_wall_2 = nullptr;
	GameObject* m_p_locked_wall_3 = nullptr;

	//Stage Normal Walls
	GameObject* m_p_normal_wall_1 = nullptr;
	GameObject* m_p_normal_wall_2 = nullptr;
	GameObject* m_p_normal_wall_3 = nullptr;

	//Camera Script
	Camera_Script* p_camera_script = nullptr;

	//Prefab
	std::shared_ptr<Prefab> m_p_z_prefab = nullptr;
	std::shared_ptr<Prefab> m_p_colonel_prefab = nullptr;

	//GameObject
	GameObject* m_p_z_game_object = nullptr;
	Z_Script* m_p_z_script = nullptr;

	GameObject* m_p_colonel_game_object = nullptr;
	Colonel_Script* m_p_colonel_script = nullptr;

	Vector3 m_stage1_boss_spawn = Vector3::Zero;
	Vector3 m_stage2_boss_spawn = Vector3::Zero;

	bool m_ready_to_play = false;
	UINT m_current_stage = 0;
};

