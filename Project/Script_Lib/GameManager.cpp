#include "stdafx.h"
#include "GameManager.h"

GameManager::GameManager()
	:Script("GameManager")
{
}

GameManager::GameManager(const GameManager& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

GameManager::~GameManager()
{
}

void GameManager::Initialize()
{
}

void GameManager::Start()
{
}

void GameManager::Update()
{
}

void GameManager::SaveToScene(FILE* p_file)
{
}

void GameManager::LoadFromScene(FILE* p_file)
{
}
