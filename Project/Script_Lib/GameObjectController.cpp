#include "stdafx.h"
#include "GameObjectController.h"

#include <DX11_2D_GameEngine_Lib/AudioClip.h>

#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/AudioSource.h>

GameObjectController::~GameObjectController()
{
	m_p_transform = nullptr;
	m_p_animator2D = nullptr;
	m_p_audio_source = nullptr;

	m_p_ready_event_func = nullptr;
	m_p_win_event_func = nullptr;
	m_p_dead_event_func = nullptr;
}

void GameObjectController::SetCurrentAnimation(const std::string& animation_name, const bool& is_loop, const bool& is_reverse)
{
	if (m_p_animator2D == nullptr)
		return;

	m_p_animator2D->SetCurrentAnimation(animation_name);
	m_p_animator2D->SetIsLoop(is_loop);
	m_p_animator2D->SetIsPlayReverse(is_reverse);
}

void GameObjectController::SetCurrentAudioClip(const std::string& audio_clip_name, const float& volume, const bool& is_loop)
{
	if (m_p_audio_source == nullptr)
		return;

	auto p_current_audio_clip = m_p_audio_source->GetCurrentAudioClip();
	if (p_current_audio_clip == nullptr || !p_current_audio_clip->GetResourceName()._Equal(audio_clip_name))
		m_p_audio_source->SetCurrentAudioClip(audio_clip_name, volume, is_loop);

	else
		m_p_audio_source->Play();
}

const Vector3 GameObjectController::GetPosition()
{
	if (m_p_transform == nullptr)
		return Vector3::Zero;

	return m_p_transform->GetLocalTranslation();
}

void GameObjectController::SetPosition(const Vector3 position)
{
	if (m_p_transform == nullptr)
		return;

	m_p_transform->SetLocalTranslation(position);
}

const bool GameObjectController::GetCurrentAnimationIsFinished()
{
	if (m_p_audio_source == nullptr)
		return false;

	return m_p_animator2D->GetCurrentAnimationIsFinished();
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
