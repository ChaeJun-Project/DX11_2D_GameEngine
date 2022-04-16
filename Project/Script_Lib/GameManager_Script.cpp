#include "stdafx.h"
#include "GameManager_Script.h"

#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>

//UI
#include "Ready_Script.h";
#include "Warning_Script.h";
#include "Hp_Script.h"

//Stage Event
#include "StageEvent_Script.h"

//Camera Script
#include "Camera_Script.h"

//Player Script
#include "Z_Script.h"

//Enemy Script
#include "Colonel_Script.h"

GameManager_Script::GameManager_Script()
	:Script("GameManager_Script")
{
	RegisterScriptParamData();

}

GameManager_Script::GameManager_Script(const GameManager_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	m_is_active = origin.m_is_active;

	m_p_z_prefab = origin.m_p_z_prefab;
	m_p_colonel_prefab = origin.m_p_colonel_prefab;

	m_stage1_boss_spawn = origin.m_stage1_boss_spawn;
	m_stage2_boss_spawn = origin.m_stage2_boss_spawn;
}

GameManager_Script::~GameManager_Script()
{
    //Audio
	m_p_audio_source = nullptr;

	//UI
	m_p_ready_ui = nullptr;
	m_p_ready_script = nullptr;

	m_p_warning_ui = nullptr;
	m_p_warning_script = nullptr;

	m_p_player_hp_gage = nullptr;
	m_p_boss_hp_gage = nullptr;

	//Stage Events
	m_p_stage_event_1 = nullptr;
	m_p_stage_event_2 = nullptr;

	//Stage Locked Walls
	m_p_locked_wall_1 = nullptr;
	m_p_locked_wall_2 = nullptr;
	m_p_locked_wall_3 = nullptr;

	//Stage Normal Walls
	m_p_normal_wall_1 = nullptr;
	m_p_normal_wall_2 = nullptr;
	m_p_normal_wall_3 = nullptr;

	//Prefab
	m_p_z_prefab = nullptr;
	m_p_colonel_prefab = nullptr;

	//GameObject
	m_p_z_game_object = nullptr;
	m_p_z_script = nullptr;

	m_p_colonel_game_object = nullptr;
	m_p_colonel_script = nullptr;
}

void GameManager_Script::Awake()
{
	m_p_audio_source = m_p_owner_game_object->GetComponent<AudioSource>();
}

void GameManager_Script::Start()
{
	//UI
	SetUI();

	//Stage Events
	SetStageEvent();

	//Stage Locked Walls
	SetStageLockedWall();

	//Stage Normal Walls
	SetStageNormalWall();

	//Camera Script
	auto p_current_scene = SCENE_MANAGER->GetCurrentScene();
	p_camera_script = dynamic_cast<Camera_Script*>(p_current_scene->FindGameObjectWithName("Main Camera")->GetScript("Camera_Script"));

    //Stage BGM Play
	m_p_audio_source->Play();
}

void GameManager_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Z", ScriptParamType::Prefab, &m_p_z_prefab, 100.f));
	AddScriptParamData(ScriptParamStruct("Colonel", ScriptParamType::Prefab, &m_p_colonel_prefab, 100.f));

	AddScriptParamData(ScriptParamStruct("Stage1 Boss Spawn", ScriptParamType::Vector3, reinterpret_cast<void*>(&m_stage1_boss_spawn), 130.f));
	AddScriptParamData(ScriptParamStruct("Stage2 Boss Spawn", ScriptParamType::Vector3, reinterpret_cast<void*>(&m_stage2_boss_spawn), 130.f));
}

void GameManager_Script::SetUI()
{
	auto p_current_scene = SCENE_MANAGER->GetCurrentScene();

	auto p_canvas = p_current_scene->FindGameObjectWithName("Canvas");
	//UI
	//Ready UI
	m_p_ready_ui = p_canvas->GetChildFromIndex(0);
	m_p_ready_script = dynamic_cast<Ready_Script*>(m_p_ready_ui->GetScript("Ready_Script"));
	m_p_ready_script->SetEndEventFunc(std::bind(&GameManager_Script::ReadyToPlay, this));
	m_p_ready_ui->SetIsActive(true);

	//Warning UI
	m_p_warning_ui = p_canvas->GetChildFromIndex(1);
	m_p_warning_script = dynamic_cast<Warning_Script*>(m_p_warning_ui->GetScript("Warning_Script"));
	m_p_warning_script->SetEndEventFunc(std::bind(&GameManager_Script::CreateBoss, this));

	//Player Hp Game
	m_p_player_hp_gage = p_canvas->GetChildFromIndex(2);
	m_p_player_hp_script = dynamic_cast<Hp_Script*>(m_p_player_hp_gage->GetScript("Hp_Script"));

	m_p_boss_hp_gage = p_canvas->GetChildFromIndex(3);
	m_p_boss_hp_script = dynamic_cast<Hp_Script*>(m_p_boss_hp_gage->GetScript("Hp_Script"));
}

void GameManager_Script::SetStageEvent()
{
	auto p_current_scene = SCENE_MANAGER->GetCurrentScene();

	auto p_stage_events = p_current_scene->FindGameObjectWithName("Stage Events");
	//Stage Events
	m_p_stage_event_1 = p_stage_events->GetChildFromIndex(0);
	auto p_stage_event_script = dynamic_cast<StageEvent_Script*>(m_p_stage_event_1->GetScript("StageEvent_Script"));
	p_stage_event_script->SetEventFunc(std::bind(&GameManager_Script::ReadyStage1, this));

	m_p_stage_event_2 = p_stage_events->GetChildFromIndex(1);
	p_stage_event_script = dynamic_cast<StageEvent_Script*>(m_p_stage_event_2->GetScript("StageEvent_Script"));
	p_stage_event_script->SetEventFunc(std::bind(&GameManager_Script::ReadyStage2, this));
}

void GameManager_Script::SetStageLockedWall()
{
	auto p_current_scene = SCENE_MANAGER->GetCurrentScene();

	auto p_stage_locked_walls = p_current_scene->FindGameObjectWithName("Stage Locked Walls");
	//Stage1 Locked Walls
	m_p_locked_wall_1 = p_stage_locked_walls->GetChildFromIndex(0);
	m_p_locked_wall_2 = p_stage_locked_walls->GetChildFromIndex(1);
	//Stage2 Locked Walls
	m_p_locked_wall_3 = p_stage_locked_walls->GetChildFromIndex(2);
}

void GameManager_Script::SetStageNormalWall()
{
	auto p_current_scene = SCENE_MANAGER->GetCurrentScene();

	auto p_walls = p_current_scene->FindGameObjectWithName("Walls");

	//Stage1 Normal Walls
	m_p_normal_wall_1 = p_walls->GetChildFromIndex(1);
	m_p_normal_wall_2 = p_walls->GetChildFromIndex(2);

	//Stage2 Normal Wall
	m_p_normal_wall_3 = p_walls->GetChildFromIndex(3);
}

void GameManager_Script::ReadyToPlay()
{
	CreatePlayer();
	m_ready_to_play = true;

	LOG_INFO_F("게임이 시작되었습니다.");
}

void GameManager_Script::CreatePlayer()
{
	m_p_z_game_object = Instantiate(m_p_z_prefab, Vector3::Zero);
	m_p_z_script = dynamic_cast<Z_Script*>(m_p_z_game_object->GetScript("Z_Script"));
	m_p_z_script->SetWinEventFunc(std::bind(&GameManager_Script::EndStage, this));
	m_p_z_script->SetDeadEventFunc(std::bind(&GameManager_Script::EndStage, this));
	m_p_z_script->SetDeadEventCallWait(3.0f);

	//Set Player Hp Gage
	m_p_player_hp_gage->SetIsActive(true);
	m_p_player_hp_script->SetTargetGameObject(m_p_z_game_object);

	//Scene Camera Target을 Player로 설정
	p_camera_script->SetTarget(m_p_z_game_object);

	LOG_INFO_F("캐릭터 %s가 생성되었습니다.", m_p_z_game_object->GetGameObjectName().c_str());
}

void GameManager_Script::CreateBoss()
{
	switch (m_current_stage)
	{
	case 1:
		m_p_colonel_game_object = Instantiate(m_p_colonel_prefab, m_stage1_boss_spawn);
		m_p_colonel_script = dynamic_cast<Colonel_Script*>(m_p_colonel_game_object->GetScript("Colonel_Script"));
		m_p_colonel_script->SetDeadEventFunc(std::bind(&GameManager_Script::EndStage1, this));
		LOG_INFO_F("스테이지1의 보스가 생성되었습니다.");
		break;
	case 2:
		m_p_colonel_game_object = Instantiate(m_p_colonel_prefab, m_stage2_boss_spawn);
		m_p_colonel_script = dynamic_cast<Colonel_Script*>(m_p_colonel_game_object->GetScript("Colonel_Script"));
		m_p_colonel_script->SetDeadEventFunc(std::bind(&GameManager_Script::PlayerWin, this));
		LOG_INFO_F("스테이지2의 보스가 생성되었습니다.");
		break;
	}

	m_p_colonel_script->SetReadyEventFunc(std::bind(&GameManager_Script::StartStage, this));
	m_p_colonel_script->SetDeadEventCallWait(8.0f);

	//Set Boss Hp Gage
	m_p_boss_hp_gage->SetIsActive(true);
	m_p_boss_hp_script->SetTargetGameObject(m_p_colonel_game_object);

	//Scene Camera Target을 Boss로 설정
	p_camera_script->SetTarget(m_p_colonel_game_object);
}

void GameManager_Script::ReadyStage1()
{
	++m_current_stage;

	m_p_locked_wall_1->SetIsActive(true);
	m_p_locked_wall_2->SetIsActive(true);

	m_p_normal_wall_1->SetIsActive(false);
	m_p_normal_wall_2->SetIsActive(false);

	m_p_warning_ui->SetIsActive(true);

	m_p_z_script->SetIsActive(false);

	LOG_INFO_F("플레이어가 스테이지1을 시작했습니다.");
}

void GameManager_Script::EndStage1()
{
	m_p_locked_wall_1->SetIsActive(false);
	m_p_locked_wall_2->SetIsActive(false);

	m_p_normal_wall_1->SetIsActive(true);
	m_p_normal_wall_2->SetIsActive(true);

	OnDestroy(m_p_colonel_game_object);

	m_p_boss_hp_gage->SetIsActive(false);

	LOG_INFO_F("플레이어가 스테이지1을 클리어했습니다.");
}

void GameManager_Script::ReadyStage2()
{
	++m_current_stage;

	m_p_locked_wall_3->SetIsActive(true);
	m_p_normal_wall_3->SetIsActive(false);

	m_p_warning_ui->SetIsActive(true);

	m_p_z_script->SetIsActive(false);

	LOG_INFO_F("플레이어가 스테이지2를 시작했습니다.");
}

void GameManager_Script::StartStage()
{
	m_p_z_script->SetIsActive(true);

	//Scene Camera Target을 Player로 설정
	p_camera_script->SetTarget(m_p_z_game_object);
}

void GameManager_Script::PlayerWin()
{
	OnDestroy(m_p_colonel_game_object);

	m_p_boss_hp_gage->SetIsActive(false);

	m_p_z_script->SetPlayerWin(true);

	LOG_INFO_F("플레이어가 모든 스테이지를 클리어했습니다.");
}

void GameManager_Script::EndStage()
{
	LOG_INFO_F("메뉴 페이지로 이동합니다.");

	std::thread loading_thread(&GameManager_Script::LoadScene, this);

	//로딩 스레드가 종료될 때까지 기다리지 않음(메인 스레드는 계속 작동)
	loading_thread.detach();
}

void GameManager_Script::LoadScene()
{
	std::cout << "[Game Title]" << std::endl;
	std::cout << "Scene Load Thread Start" << std::endl;

	auto next_scene = SCENE_MANAGER->LoadScene((FILE_MANAGER->GetAbsoluteContentPath() + "Asset/Scene/Game Title.scene"));

	//Change Scene
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Scene_Change;
	event_struct.object_address_1 = next_scene;

	EVENT_MANAGER->AddEvent(event_struct);

	std::cout << "Scene Load Thread End" << std::endl;

	LOG_INFO_F("'%s' Scene이 성공적으로 로드되었습니다.", next_scene->GetSceneName().c_str());
}

void GameManager_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script

	//Prefab
	fprintf_s(p_file, "[Z]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(m_p_z_prefab, p_file);

	fprintf_s(p_file, "[Colonel]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(m_p_colonel_prefab, p_file);

	//Boss Spawn
	fprintf_s(p_file, "[Stage1 Boss Spawn Position]\n");
	FILE_MANAGER->FPrintf_Vector3(m_stage1_boss_spawn, p_file);

	fprintf_s(p_file, "[Stage2 Boss Spawn Position]\n");
	FILE_MANAGER->FPrintf_Vector3(m_stage2_boss_spawn, p_file);
}

void GameManager_Script::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = { 0 };

	//Prefab
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Z]
	RESOURCE_MANAGER->LoadResource<Prefab>(m_p_z_prefab, p_file);

	FILE_MANAGER->FScanf(char_buffer, p_file); //[Colonel]
	RESOURCE_MANAGER->LoadResource<Prefab>(m_p_colonel_prefab, p_file);

	//Boss Spawn
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Stage1 Boss Spawn Position]
	FILE_MANAGER->FScanf_Vector3(m_stage1_boss_spawn, p_file);

	FILE_MANAGER->FScanf(char_buffer, p_file); //[Stage2 Boss Spawn Position]
	FILE_MANAGER->FScanf_Vector3(m_stage2_boss_spawn, p_file);
}
