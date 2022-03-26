#include "stdafx.h"
#include "GameObjectController.h"

#include <DX11_2D_GameEngine_Lib/Animator2D.h>

GameObjectController::~GameObjectController()
{
	m_p_animator2D = nullptr;
}

void GameObjectController::SetCurrentAnimation(const std::string& animation_name, const bool& is_loop, const bool& is_reverse)
{
	if (m_p_animator2D == nullptr)
		return;

	m_p_animator2D->SetCurrentAnimation(animation_name);
	m_p_animator2D->SetIsLoop(is_loop);
	m_p_animator2D->SetIsPlayReverse(is_reverse);
}

void GameObjectController::SaveToScene(FILE* p_file)
{
    //Hp
	fprintf_s(p_file, "[Hp]\n");
	fprintf_s(p_file, "%d\n", m_hp);

	//Run Speed
	fprintf_s(p_file, "[Run Speed]\n");
	fprintf_s(p_file, "%f\n", m_run_speed);

	//Jump Speed
	fprintf_s(p_file, "[Jump Speed]\n");
	fprintf_s(p_file, "%f\n", m_jump_speed);

	//Dash Speed
	fprintf_s(p_file, "[Dash Speed]\n");
	fprintf_s(p_file, "%f\n", m_dash_speed);
}

void GameObjectController::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = { 0 };

	//Hp
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &m_hp);

	//Speed
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_run_speed);

	//Jump Speed
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_jump_speed);

	//Dash Speed
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_dash_speed);
}
