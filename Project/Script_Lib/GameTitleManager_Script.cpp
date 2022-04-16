#include "stdafx.h"
#include "GameTitleManager_Script.h"

#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>

#include "Menu_Script.h"
#include "PlayerSelect_Script.h"
#include "Loading_Script.h"

GameTitleManager_Script::GameTitleManager_Script()
	:Script("GameTitleManager_Script")
{
}

GameTitleManager_Script::GameTitleManager_Script(const GameTitleManager_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

GameTitleManager_Script::~GameTitleManager_Script()
{
#pragma region Menu
	m_p_menu_page = nullptr;
	m_p_menu_script = nullptr;
#pragma endregion

#pragma region Player Select
	m_p_player_select_page = nullptr;
	m_p_player_select_script = nullptr;
#pragma endregion

#pragma region Loading
	m_p_loading_page = nullptr;
	m_p_loading_script = nullptr;
#pragma endregion
}

void GameTitleManager_Script::Start()
{
	auto p_curret_scene = SCENE_MANAGER->GetCurrentScene();

	auto p_canvas = p_curret_scene->FindGameObjectWithName("Canvas");

#pragma region Menu
	m_p_menu_page = p_canvas->GetChildFromIndex(0);
	m_p_menu_script = dynamic_cast<Menu_Script*>(m_p_menu_page->GetScript("Menu_Script"));
	m_p_menu_script->SetActiveLoadingPageCallBack(std::bind(&GameTitleManager_Script::ActiveLoadingPage, this));
	m_p_menu_script->SetActivePlayerSelectPageCallBack(std::bind(&GameTitleManager_Script::ActivePlayerSelectPage, this));
#pragma endregion

#pragma region Player Select
	m_p_player_select_page = p_canvas->GetChildFromIndex(1);
	m_p_player_select_script = dynamic_cast<PlayerSelect_Script*>(m_p_player_select_page->GetScript("PlayerSelect_Script"));
	m_p_player_select_script->SetActiveLoadingPageCallBack(std::bind(&GameTitleManager_Script::ActiveLoadingPage, this));
	m_p_player_select_script->SetActiveMenuPageCallBack(std::bind(&GameTitleManager_Script::ActiveMenuPage, this));
#pragma endregion

#pragma region Loading
	m_p_loading_page = p_canvas->GetChildFromIndex(2);
	m_p_loading_script = dynamic_cast<Loading_Script*>(m_p_loading_page->GetScript("Loading_Script"));
#pragma endregion

	m_p_current_page = m_p_menu_page;
}

void GameTitleManager_Script::ActiveMenuPage()
{
	//���� �������� ��Ȱ��ȭ�ϰ�
	if (m_p_current_page != nullptr)
		m_p_current_page->SetIsActive(false);

	//���� �������� Menu �������� ����
	m_p_current_page = m_p_menu_page;
	m_p_current_page->SetIsActive(true);

	LOG_INFO_F("�÷��̾ �޴� �������� �̵��߽��ϴ�.");
}

void GameTitleManager_Script::ActivePlayerSelectPage()
{
	//���� �������� ��Ȱ��ȭ�ϰ�
	if (m_p_current_page != nullptr)
		m_p_current_page->SetIsActive(false);

	//���� �������� Player Select �������� ����
	m_p_current_page = m_p_player_select_page;
	m_p_current_page->SetIsActive(true);

	LOG_INFO_F("�÷��̾ ĳ���� ���� �������� �̵��߽��ϴ�.");
}

void GameTitleManager_Script::ActiveLoadingPage()
{
	//���� �������� ��Ȱ��ȭ�ϰ�
	if (m_p_current_page != nullptr)
		m_p_current_page->SetIsActive(false);

	//���� �������� Loading �������� ����
	m_p_current_page = m_p_loading_page;
	m_p_current_page->SetIsActive(true);

	LOG_INFO_F("�÷��̾ �ε� �������� �̵��߽��ϴ�.");
}

void GameTitleManager_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}

void GameTitleManager_Script::LoadFromScene(FILE* p_file)
{
}
