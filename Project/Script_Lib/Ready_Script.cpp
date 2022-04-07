#include "stdafx.h"
#include "Ready_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>

Ready_Script::Ready_Script()
	:Script("Ready_Script")
{
	RegisterScriptParamData();

	m_textrue_path = "UI/Ready";
	m_textrue_path = FILE_MANAGER->GetAbsoluteTexturePath() + m_textrue_path;

	LoadTextures();
}

Ready_Script::Ready_Script(const Ready_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	m_is_active = origin.m_is_active;

	m_textrue_path = origin.m_textrue_path;

	for (const auto& p_texture : origin.m_p_textrue_vector)
	{
		m_p_textrue_vector.emplace_back(p_texture);
	}

	m_duration = origin.m_duration;
}

void Ready_Script::Awake()
{
	UIAnimationController::p_image_renderer = m_p_owner_game_object->GetComponent<ImageRenderer>();
	UIAnimationController::m_p_audio_source = m_p_owner_game_object->GetComponent<AudioSource>();
}

void Ready_Script::Start()
{
	Play();
}

void Ready_Script::Update()
{
	UpdateUIAnimation();

	if(!m_is_playing)
	  m_p_owner_game_object->SetIsActive(false);
}

void Ready_Script::Play()
{
	PlayUIAnimation();
	SetCurrentAudioClip("Ready_Sound", 0.5f);
}

void Ready_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Duration", ScriptParamType::Float, reinterpret_cast<void*>(&m_duration), 100.0f));
}

void Ready_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script

	UIAnimationController::SaveToScene(p_file); //UIAnimationController
}

void Ready_Script::LoadFromScene(FILE* p_file)
{
	UIAnimationController::LoadFromScene(p_file); //UIAnimationController
}
