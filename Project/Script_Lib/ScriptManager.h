#pragma once

#include <vector>
#include <string>

enum class Script_Type : UINT
{
	Camera_Script,
	Ceiling_Script,
	Colonel_Script,
	GameManager_Script,
	Ground_Script,
	Hp_Script,
	WalkCannon_Bullet_Script,
	WalkCannon_Script,
	Wall_Script,
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
