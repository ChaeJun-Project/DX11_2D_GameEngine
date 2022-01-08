#include "stdafx.h"
#include "Animation2D.h"

#include "GraphicsManager.h"

#include "Animator2D.h"

#include "Texture.h"

Animation2D::Animation2D(const Animation2D& origin)
{
}

Animation2D::~Animation2D()
{
	m_p_owner_animator2D = nullptr;

	m_p_animation_frame_vector.clear();
	m_p_animation_frame_vector.shrink_to_fit();

	m_p_atlas_texture.reset();
}

void Animation2D::LoadFromFile(const std::string& animation_directory_path)
{
}

void Animation2D::SaveFile(const std::string& animation_directory_path)
{
}

void Animation2D::Update()
{
	if (m_p_owner_animator2D == nullptr || m_p_animation_frame_vector.empty()|| m_is_paused || m_is_finished)
		return;

	auto animation_speed = m_p_owner_animator2D->GetAnimationSpeed();
	auto delta_time_f = DELTA_TIME_F;
	//시간 누적
	m_accumulate_time += (delta_time_f * animation_speed);

	//해당 프레임의 유지 시간이 경과한 경우
	if (m_p_animation_frame_vector[m_current_frame_id].duration <= m_accumulate_time)
	{
		m_accumulate_time = 0.0f;

		//애니메이션 역재생인 경우
		if (m_p_owner_animator2D->GetIsPlayReverse())
		{
			--m_current_frame_id;
			//애니메이션 역재생이 끝났을 때
			if (m_current_frame_id <= 0)
			{
				m_current_frame_id = static_cast<int>(m_p_animation_frame_vector.size());

				//해당 애니메이션이 끝까지 1회 재생되었음을 알려줌
				m_is_finished = true;
			}
		}

		//애니메이션이 정방향 재생인 경우
		else
		{
			++m_current_frame_id;
			//애니메이션 재생이 끝났을 때
			if (m_current_frame_id >= m_p_animation_frame_vector.size())
			{
				m_current_frame_id = 0;

				//해당 애니메이션이 끝까지 1회 재생되었음을 알려줌
				m_is_finished = true;
			}
		}

		DoAnimationEvent(static_cast<UINT>(m_current_frame_id));
	}
}

void Animation2D::FinalUpdate()
{
	if (m_p_owner_animator2D == nullptr || m_p_animation_frame_vector.empty())
		return;

	auto animator2D_index = m_p_owner_animator2D->GetAnimator2DID();

	Animation2D_Data animation2D_data = m_p_animation_frame_vector[m_current_frame_id].animation2D_data;

	Vector2 atlas_texture_size = Vector2(static_cast<float>(m_p_atlas_texture->GetWidth()), static_cast<float>(m_p_atlas_texture->GetHeight()));

	animation2D_data.left_top = Vector2(animation2D_data.left_top.x / atlas_texture_size.x, animation2D_data.left_top.y / atlas_texture_size.y);
	animation2D_data.frame_size = Vector2(animation2D_data.frame_size.x / atlas_texture_size.x, animation2D_data.frame_size.y / atlas_texture_size.y);
	animation2D_data.full_frame_size = Vector2(animation2D_data.full_frame_size.x / atlas_texture_size.x, animation2D_data.full_frame_size.y / atlas_texture_size.y);

	g_cbuffer_animation2D.animation2D_data_array[animator2D_index] = animation2D_data;

	auto constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::Animation2D);
	constant_buffer->SetConstantBufferData(&g_cbuffer_animation2D, sizeof(CBuffer_Animation2D));
	constant_buffer->SetBufferBindStage(PipelineStage::PS); //애니메이션 픽셀계산은 Pixel Shader에서만 수행
	constant_buffer->BindPipeline();
}

void Animation2D::Play()
{
	m_is_paused = false;
	m_is_finished = false;

	//정방향 재생인 경우
	if (!m_p_owner_animator2D->GetIsPlayReverse())
		m_current_frame_id = 0;

	//역방향 재생인 경우
	else
		m_current_frame_id = static_cast<int>(m_p_animation_frame_vector.size());
}

void Animation2D::Pause()
{
	m_is_paused = true;
}

void Animation2D::Stop()
{
	m_is_paused = false;
	m_is_finished = false;

	m_current_frame_id = 0;
	ResetAnimationEventFlag();
}

void Animation2D::SetAnimationEvent(const UINT& clip_index, std::function<void(void)> event_func)
{
	auto map_iter = m_animation_event_func_map.insert(std::make_pair(clip_index, std::make_pair(false, event_func)));
	auto result = map_iter.second;
	if (!result)
		return;
}

void Animation2D::DoAnimationEvent(const UINT& clip_index)
{
	auto animation_event_iter = m_animation_event_func_map.find(clip_index);
	if (animation_event_iter != m_animation_event_func_map.end())
	{
		auto animation_event = animation_event_iter->second;
		//이미 수행한 경우
		if (animation_event.first)
			return;

		animation_event.first = true;
		animation_event.second();
	}
}

void Animation2D::ResetAnimationEventFlag()
{
	for (auto& animation_event : m_animation_event_func_map)
	{
		animation_event.second.first = false;
	}
}

void Animation2D::SaveToScene(FILE* p_file)
{
}

void Animation2D::LoadFromScene(FILE* p_file)
{
}
