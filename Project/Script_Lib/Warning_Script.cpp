#include "stdafx.h"
#include "Warning_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>

Warning_Script::Warning_Script()
	:Script("Warning_Script")
{
	RegisterScriptParamData();

	m_textrue_path = "UI/Warning";
	m_textrue_path = FILE_MANAGER->GetAbsoluteTexturePath() + m_textrue_path;

	LoadTextures();
}

Warning_Script::Warning_Script(const Warning_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	m_textrue_path = origin.m_textrue_path;

	for (const auto& p_texture : origin.m_p_textrue_vector)
	{
		m_p_textrue_vector.emplace_back(p_texture);
	}

	m_duration = origin.m_duration;
}

void Warning_Script::Start()
{
	UIAnimationController::p_image_renderer = m_p_owner_game_object->GetComponent<ImageRenderer>();
	UIAnimationController::m_p_audio_source = m_p_owner_game_object->GetComponent<AudioSource>();

	Play();
}

void Warning_Script::Update()
{
	UpdateUIAnimation();

	if (m_is_finished && m_p_owner_game_object->GetIsActive())
	{
		StopUIAnimation();
		m_p_owner_game_object->SetIsActive(false);
	}
}

void Warning_Script::Play()
{
	PlayUIAnimation();
	SetCurrentAudioClip("Warning_Sound", 0.5f, true);
}

void Warning_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Duration", ScriptParamType::Float, reinterpret_cast<void*>(&m_duration), 100.0f));
}

void Warning_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script

	UIAnimationController::SaveToScene(p_file); //UIAnimationController
}

void Warning_Script::LoadFromScene(FILE* p_file)
{
	UIAnimationController::LoadFromScene(p_file); //UIAnimationController
}