#include "stdafx.h"
#include "ScriptManager.h"

#include "GameManager.h"
#include "WalkCannon_Bullet_Script.h"
#include "WalkCannon_Script.h"
#include "X_Script.h"
#include "Z_Script.h"

void ScriptManager::GetScriptInfo(std::vector<std::string>& script_vector)
{
	script_vector.emplace_back("GameManager");
	script_vector.emplace_back("WalkCannon_Bullet_Script");
	script_vector.emplace_back("WalkCannon_Script");
	script_vector.emplace_back("X_Script");
	script_vector.emplace_back("Z_Script");
}

Script* ScriptManager::GetScript(const std::string& script_name)
{
	if(script_name._Equal("GameManager"))
		return new GameManager;
	if(script_name._Equal("WalkCannon_Bullet_Script"))
		return new WalkCannon_Bullet_Script;
	if(script_name._Equal("WalkCannon_Script"))
		return new WalkCannon_Script;
	if(script_name._Equal("X_Script"))
		return new X_Script;
	if(script_name._Equal("Z_Script"))
		return new Z_Script;

	return nullptr;
}

Script* ScriptManager::GetScript(const UINT& script_type)
{
	switch (script_type)
	{
	case (UINT)Script_Type::GameManager:
		return new GameManager;
	case (UINT)Script_Type::WalkCannon_Bullet_Script:
		return new WalkCannon_Bullet_Script;
	case (UINT)Script_Type::WalkCannon_Script:
		return new WalkCannon_Script;
	case (UINT)Script_Type::X_Script:
		return new X_Script;
	case (UINT)Script_Type::Z_Script:
		return new Z_Script;
	}
	return nullptr; 
}

