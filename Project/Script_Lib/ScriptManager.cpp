#include "stdafx.h"
#include "ScriptManager.h"

#include "Camera_Script.h"
#include "Ceiling_Script.h"
#include "Colonel_Attack1_Effect_Script.h"
#include "Colonel_Attack2_Effect_Script.h"
#include "Colonel_Attack3_Effect_Script.h"
#include "Colonel_Script.h"
#include "Door_Script.h"
#include "ExplosionEffectManager_Script.h"
#include "ExplosionEffect_Script.h"
#include "GameManager_Script.h"
#include "Ground_Script.h"
#include "Hp_Script.h"
#include "Ready_Script.h"
#include "StageEvent_Script.h"
#include "Wall_Script.h"
#include "Warning_Script.h"
#include "X_Script.h"
#include "Z_Script.h"

void ScriptManager::GetScriptInfo(std::vector<std::string>& script_vector)
{
	script_vector.emplace_back("Camera_Script");
	script_vector.emplace_back("Ceiling_Script");
	script_vector.emplace_back("Colonel_Attack1_Effect_Script");
	script_vector.emplace_back("Colonel_Attack2_Effect_Script");
	script_vector.emplace_back("Colonel_Attack3_Effect_Script");
	script_vector.emplace_back("Colonel_Script");
	script_vector.emplace_back("Door_Script");
	script_vector.emplace_back("ExplosionEffectManager_Script");
	script_vector.emplace_back("ExplosionEffect_Script");
	script_vector.emplace_back("GameManager_Script");
	script_vector.emplace_back("Ground_Script");
	script_vector.emplace_back("Hp_Script");
	script_vector.emplace_back("Ready_Script");
	script_vector.emplace_back("StageEvent_Script");
	script_vector.emplace_back("Wall_Script");
	script_vector.emplace_back("Warning_Script");
	script_vector.emplace_back("X_Script");
	script_vector.emplace_back("Z_Script");
}

Script* ScriptManager::GetScript(const std::string& script_name)
{
	if(script_name._Equal("Camera_Script"))
		return new Camera_Script;
	if(script_name._Equal("Ceiling_Script"))
		return new Ceiling_Script;
	if(script_name._Equal("Colonel_Attack1_Effect_Script"))
		return new Colonel_Attack1_Effect_Script;
	if(script_name._Equal("Colonel_Attack2_Effect_Script"))
		return new Colonel_Attack2_Effect_Script;
	if(script_name._Equal("Colonel_Attack3_Effect_Script"))
		return new Colonel_Attack3_Effect_Script;
	if(script_name._Equal("Colonel_Script"))
		return new Colonel_Script;
	if(script_name._Equal("Door_Script"))
		return new Door_Script;
	if(script_name._Equal("ExplosionEffectManager_Script"))
		return new ExplosionEffectManager_Script;
	if(script_name._Equal("ExplosionEffect_Script"))
		return new ExplosionEffect_Script;
	if(script_name._Equal("GameManager_Script"))
		return new GameManager_Script;
	if(script_name._Equal("Ground_Script"))
		return new Ground_Script;
	if(script_name._Equal("Hp_Script"))
		return new Hp_Script;
	if(script_name._Equal("Ready_Script"))
		return new Ready_Script;
	if(script_name._Equal("StageEvent_Script"))
		return new StageEvent_Script;
	if(script_name._Equal("Wall_Script"))
		return new Wall_Script;
	if(script_name._Equal("Warning_Script"))
		return new Warning_Script;
	if(script_name._Equal("X_Script"))
		return new X_Script;
	if(script_name._Equal("Z_Script"))
		return new Z_Script;

	return nullptr;
}

