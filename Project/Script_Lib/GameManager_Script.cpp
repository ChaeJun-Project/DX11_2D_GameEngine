#include "stdafx.h"
#include "GameManager_Script.h"

GameManager_Script::GameManager_Script()
	:Script("GameManager_Script")
{
}

GameManager_Script::GameManager_Script(const GameManager_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

GameManager_Script::~GameManager_Script()
{
}

void GameManager_Script::Start()
{
}

void GameManager_Script::Update()
{
}

void GameManager_Script::SaveToScene(FILE* p_file)
{
}

void GameManager_Script::LoadFromScene(FILE* p_file)
{
}
