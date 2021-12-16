#include "stdafx.h"
#include "ScriptManager.h"

#include "GameLogic_Script.h"
#include "Geometry_Script.h"
#include "RockManZ_Script.h"
#include "WalkCannon_Bullet_Script.h"
#include "WalkCannon_Script.h"

void ScriptManager::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"GameLogic_Script");
	_vec.push_back(L"Geometry_Script");
	_vec.push_back(L"RockManZ_Script");
	_vec.push_back(L"WalkCannon_Bullet_Script");
	_vec.push_back(L"WalkCannon_Script");
}

Script * ScriptManager::GetScript(const wstring& _strScriptName)
{
	if (L"GameLogic_Script" == _strScriptName)
		return new GameLogic_Script;
	if (L"Geometry_Script" == _strScriptName)
		return new Geometry_Script;
	if (L"RockManZ_Script" == _strScriptName)
		return new RockManZ_Script;
	if (L"WalkCannon_Bullet_Script" == _strScriptName)
		return new WalkCannon_Bullet_Script;
	if (L"WalkCannon_Script" == _strScriptName)
		return new WalkCannon_Script;
	return nullptr;
}

Script * ScriptManager::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::GAMELOGIC_SCRIPT:
		return new GameLogic_Script;
		break;
	case (UINT)SCRIPT_TYPE::GEOMETRY_SCRIPT:
		return new Geometry_Script;
		break;
	case (UINT)SCRIPT_TYPE::ROCKMANZ_SCRIPT:
		return new RockManZ_Script;
		break;
	case (UINT)SCRIPT_TYPE::WALKCANNON_BULLET_SCRIPT:
		return new WalkCannon_Bullet_Script;
		break;
	case (UINT)SCRIPT_TYPE::WALKCANNON_SCRIPT:
		return new WalkCannon_Script;
		break;
	}
	return nullptr;
}

const wchar_t * ScriptManager::GetScriptName(Script * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::GAMELOGIC_SCRIPT:
		return L"GameLogic_Script";
		break;

	case SCRIPT_TYPE::GEOMETRY_SCRIPT:
		return L"Geometry_Script";
		break;

	case SCRIPT_TYPE::ROCKMANZ_SCRIPT:
		return L"RockManZ_Script";
		break;

	case SCRIPT_TYPE::WALKCANNON_BULLET_SCRIPT:
		return L"WalkCannon_Bullet_Script";
		break;

	case SCRIPT_TYPE::WALKCANNON_SCRIPT:
		return L"WalkCannon_Script";
		break;

	}
	return nullptr;
}