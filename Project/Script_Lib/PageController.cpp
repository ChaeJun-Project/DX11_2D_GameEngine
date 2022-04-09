#include "stdafx.h"
#include "PageController.h"

#include <DX11_2D_GameEngine_Lib/AudioClip.h>
#include <DX11_2D_GameEngine_Lib/AudioSource.h>

PageController::~PageController()
{
#pragma region Page Event CallBack
	m_p_active_menu_page_func = nullptr;
	m_p_active_player_select_page_func = nullptr;
	m_p_active_loading_page_func = nullptr;
#pragma endregion

#pragma region Audio Source
	m_p_audio_source = nullptr;
#pragma endregion
}

void PageController::SetCurrentAudioClip(const std::string& audio_clip_name, const float& volume, const bool& is_loop)
{
	if (m_p_audio_source == nullptr)
		return;

	auto p_current_audio_clip = m_p_audio_source->GetCurrentAudioClip();
	if (p_current_audio_clip == nullptr || !p_current_audio_clip->GetResourceName()._Equal(audio_clip_name))
		m_p_audio_source->SetCurrentAudioClip(audio_clip_name, volume, is_loop);

	else
		m_p_audio_source->Play();
}