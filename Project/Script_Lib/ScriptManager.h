#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	GAMELOGIC_SCRIPT,
	GEOMETRY_SCRIPT,
	ROCKMANZ_SCRIPT,
	WALKCANNON_BULLET_SCRIPT,
	WALKCANNON_SCRIPT,
	END,
};

using namespace std;

class Script;

class ScriptManager
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static Script * GetScript(const wstring& _strScriptName);
	static Script * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(Script * _pScript);
};
