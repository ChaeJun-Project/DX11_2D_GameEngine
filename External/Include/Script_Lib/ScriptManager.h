#pragma once

#include <vector>
#include <string>

enum class Script_Type : UINT
{
	Camera_Script,
	Ceiling_Script,
	Colonel_Attack1_Effect_Script,
	Colonel_Attack2_Effect_Script,
	Colonel_Attack3_Effect_Script,
	Colonel_Script,
	Door_Script,
	ExplosionEffectManager_Script,
	ExplosionEffect_Script,
	GameManager_Script,
	GameTitleManager_Script,
	Ground_Script,
	Hp_Script,
	Loading_Script,
	Menu_Script,
	PlayerSelect_Script,
	Ready_Script,
	StageEvent_Script,
	Wall_Script,
	Warning_Script,
	X_Script,
	Z_Script,
};

class Script;

class ScriptManager final
{
public:
	static void GetScriptInfo(std::vector<std::string>& script_vector);
	static Script* GetScript(const std::string& script_name);
};
