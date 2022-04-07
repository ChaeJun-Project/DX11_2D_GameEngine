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
	m_is_active = origin.m_is_active;

	RegisterScriptParamData();
}

GameManager_Script::~GameManager_Script()
{
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

	//Prefab
	m_p_z_prefab = nullptr;
	m_p_colonel_prefab = nullptr;

	//GameObject
	m_p_z_game_object = nullptr;
	m_p_z_script = nullptr;

	m_p_colonel_game_object = nullptr;
	m_p_colonel_script = nullptr;
}

void GameManager_Script::Start()
{
	//UI
	SetUI();

	//Stage Events
	SetStageEvent();

	//Stage Locked Walls
	SetStageLockedWall();

	//Camera Script
	auto p_current_scene = SCENE_MANAGER->GetCurrentScene();
	p_camera_script = dynamic_cast<Camera_Script*>(p_current_scene->FindGameObjectWithName("Main Camera")->GetScript("Camera_Script"));
}

void GameManager_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Z", ScriptParamType::Prefab, &m_p_z_prefab, 100.f));
	AddScriptParamData(ScriptParamStruct("Colonel", ScriptParamType::Prefab, &m_p_colonel_prefab, 100.f));
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
	p_stage_event_script->SetEventFunc(std::bind(&GameManager_Script::StartStage1, this));

	m_p_stage_event_2 = p_stage_events->GetChildFromIndex(1);
	p_stage_event_script = dynamic_cast<StageEvent_Script*>(m_p_stage_event_2->GetScript("StageEvent_Script"));
	p_stage_event_script->SetEventFunc(std::bind(&GameManager_Script::StartStage2, this));
}

void GameManager_Script::SetStageLockedWall()
{
	auto p_current_scene = SCENE_MANAGER->GetCurrentScene();

	auto p_stage_locked_walls = p_current_scene->FindGameObjectWithName("Stage Locked Walls");
	//Stage Locked Walls
	m_p_locked_wall_1 = p_stage_locked_walls->GetChildFromIndex(0);
	m_p_locked_wall_2 = p_stage_locked_walls->GetChildFromIndex(1);
	m_p_locked_wall_3 = p_stage_locked_walls->GetChildFromIndex(2);
}

void GameManager_Script::ReadyToPlay()
{
	CreatePlayer();
	m_ready_to_play = true;
}

void GameManager_Script::CreatePlayer()
{
	m_p_z_game_object = Instantiate(m_p_z_prefab, Vector3::Zero);
	m_p_z_script = dynamic_cast<Z_Script*>(m_p_z_game_object->GetScript("Z_Script"));
	m_p_z_script->SetDeadEventFunc(std::bind(&GameManager_Script::EndStage, this));
	m_p_z_script->SetDeadEventCallWait(3.0f);
	
	//Set Player Hp Gage
	m_p_player_hp_script->SetTargetGameObject(m_p_z_game_object);
	m_p_player_hp_gage->SetIsActive(true);

	//Set Scene Camera
	p_camera_script->SetTarget(m_p_z_game_object);
}

void GameManager_Script::CreateBoss()
{
	m_p_z_script->SetIsActive(true);
	
	m_p_colonel_game_object = Instantiate(m_p_colonel_prefab, Vector3::Zero);
	m_p_colonel_script = dynamic_cast<Colonel_Script*>(m_p_colonel_game_object->GetScript("Colonel_Script"));
	m_p_colonel_script->SetDeadEventFunc(std::bind(&GameManager_Script::EndStage1, this));
	m_p_colonel_script->SetDeadEventCallWait(2.0f);

	//Set Boss Hp Gage
	m_p_boss_hp_script->SetTargetGameObject(m_p_colonel_game_object);
	m_p_boss_hp_gage->SetIsActive(true);
}

void GameManager_Script::StartStage1()
{
	m_p_locked_wall_1->SetIsActive(true);
	m_p_locked_wall_2->SetIsActive(true);

	m_p_warning_ui->SetIsActive(true);

	m_p_z_script->SetIsActive(false);
}

void GameManager_Script::EndStage1()
{
	m_p_locked_wall_1->SetIsActive(false);
	m_p_locked_wall_2->SetIsActive(false);

	OnDestroy(m_p_colonel_game_object);

	m_p_boss_hp_gage->SetIsActive(false);
}

void GameManager_Script::StartStage2()
{
	m_p_locked_wall_3->SetIsActive(true);

	m_p_warning_ui->SetIsActive(true);

	m_p_z_script->SetIsActive(false);
}

void GameManager_Script::EndStage()
{
	auto next_scene = SCENE_MANAGER->LoadScene((FILE_MANAGER->GetAbsoluteContentPath() + "Asset/Scene/Game Title.scene"));

	//Change Scene
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Scene_Change;
	event_struct.object_address_1 = next_scene;

	EVENT_MANAGER->AddEvent(event_struct);
}

void GameManager_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script

	//Prefab
	fprintf_s(p_file, "[Z]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(m_p_z_prefab, p_file);

	fprintf_s(p_file, "[Colonel]\n");
	RESOURCE_MANAGER->SaveResource<Prefab>(m_p_colonel_prefab, p_file);
}

void GameManager_Script::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = { 0 };

	//Prefab
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Z]
	RESOURCE_MANAGER->LoadResource<Prefab>(m_p_z_prefab, p_file);

	FILE_MANAGER->FScanf(char_buffer, p_file); //[Colonel]
	RESOURCE_MANAGER->LoadResource<Prefab>(m_p_colonel_prefab, p_file);
}
