#include "stdafx.h"
#include "UIAnimationController.h"

#include <DX11_2D_GameEngine_Lib/Texture.h>
#include <DX11_2D_GameEngine_Lib/AudioClip.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/AudioSource.h>
#include <DX11_2D_GameEngine_Lib/ImageRenderer.h>

UIAnimationController::~UIAnimationController()
{
	//Texture Clear
	for (auto& p_texture : m_p_textrue_vector)
	{
		p_texture.reset();
	}
	m_p_textrue_vector.clear();

	//Current Texture Clear
	m_p_current_texture.reset();

	p_image_renderer = nullptr;
	m_p_audio_source = nullptr;
}

void UIAnimationController::LoadTextures()
{
	auto texture_directory_path_vector = FILE_MANAGER->GetFilesInDirectory(m_textrue_path);
	for (const auto& texture_path : texture_directory_path_vector)
	{
		m_p_textrue_vector.emplace_back(RESOURCE_MANAGER->LoadFromFile<Texture>(texture_path));
	}
}

void UIAnimationController::PlayUIAnimation()
{
	m_is_playing = true;
	m_is_finished = false;
	
	m_current_frame_id = 0;

	p_image_renderer->SetTexture(m_p_textrue_vector[m_current_frame_id]);
}

void UIAnimationController::StopUIAnimation()
{
	m_is_playing = false;

	m_current_frame_id = 0;

	p_image_renderer->SetTexture(nullptr);

	m_p_audio_source->Stop();
}

void UIAnimationController::UpdateUIAnimation()
{
	if (m_p_textrue_vector.empty() || !m_is_playing)
		return;

	//시간 누적
	m_accumulate_time += DELTA_TIME_F;

	//해당 프레임의 유지 시간이 경과한 경우
	if (m_duration <= m_accumulate_time)
	{
		m_accumulate_time = 0.0f;

		++m_current_frame_id;

		if (m_current_frame_id < m_p_textrue_vector.size())
			p_image_renderer->SetTexture(m_p_textrue_vector[m_current_frame_id]);

		//애니메이션 재생이 끝났을 때
		else
		{
			p_image_renderer->SetTexture(nullptr);

			StopUIAnimation();

			m_is_finished = true;
		}
	}
}

void UIAnimationController::SetCurrentAudioClip(const std::string& audio_clip_name, const float& volume, const bool& is_loop)
{
	auto p_current_audio_clip = m_p_audio_source->GetCurrentAudioClip();
	if (p_current_audio_clip != nullptr)
	{
		if (p_current_audio_clip->GetResourceName()._Equal(audio_clip_name))
			m_p_audio_source->Play();
	}

	m_p_audio_source->SetCurrentAudioClip(audio_clip_name, volume, is_loop);
}

void UIAnimationController::SaveToScene(FILE* p_file)
{
	//Duration
	fprintf(p_file, "[Duarion]\n");
	fprintf(p_file, "%f\n", m_duration);
}

void UIAnimationController::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = { 0 };

	//Duration
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_duration);
}
