#include "stdafx.h"
#include "SpriteAnimation.h"

#include "ConstantBuffer.h"

#include "GraphicsManager.h"

#include "Animator2D.h"

#include "Texture.h"

SpriteAnimation::SpriteAnimation(const std::string& animation2D_resource_name)
	:IResource(ResourceType::SpriteAnimation, animation2D_resource_name)
{

}

SpriteAnimation::SpriteAnimation(const SpriteAnimation& origin)
	: IResource(origin.m_resource_type, origin.m_object_name)
{
    //Resource Path
	m_resource_path = origin.m_resource_path;

	//Animator2D
	m_p_owner_animator2D = nullptr;

	//Animation Frame
	auto animation_frame_count = origin.m_animation_frame_vector.size();
	m_animation_frame_vector.reserve(animation_frame_count);
	for (const auto& animation_frame : origin.m_animation_frame_vector)
	{
		m_animation_frame_vector.emplace_back(animation_frame);
	}

	//Atlas Texture
	m_p_atlas_texture = origin.m_p_atlas_texture;
}

SpriteAnimation::~SpriteAnimation()
{
	m_p_owner_animator2D = nullptr;

	m_animation_frame_vector.clear();
	m_animation_frame_vector.shrink_to_fit();

	m_p_atlas_texture.reset();

	m_animation_event_func_multimap.clear();
}

void SpriteAnimation::Update()
{
	if (m_p_owner_animator2D == nullptr || m_animation_frame_vector.empty() || m_is_paused || m_is_finished)
		return;

	auto animation_speed = m_p_owner_animator2D->GetAnimationSpeed();
	auto delta_time_f = DELTA_TIME_F;
	//시간 누적
	m_accumulate_time += (delta_time_f * animation_speed);

	//해당 프레임의 유지 시간이 경과한 경우
	if (m_animation_frame_vector[m_current_frame_id].duration <= m_accumulate_time)
	{
		m_accumulate_time = 0.0f;

		DoAnimationEvent(static_cast<UINT>(m_current_frame_id));

		//애니메이션 역재생인 경우
		if (m_p_owner_animator2D->GetIsPlayReverse())
		{
			--m_current_frame_id;
			//애니메이션 역재생이 끝났을 때
			if (m_current_frame_id <= 0)
			{
				m_current_frame_id = (static_cast<int>(m_animation_frame_vector.size()) - 2);

				//해당 애니메이션이 끝까지 1회 재생되었음을 알려줌
				m_is_finished = true;
			}
		}

		//애니메이션이 정방향 재생인 경우
		else
		{
			++m_current_frame_id;
			//애니메이션 재생이 끝났을 때
			if (m_current_frame_id >= m_animation_frame_vector.size())
			{
				--m_current_frame_id;

				//해당 애니메이션이 끝까지 1회 재생되었음을 알려줌
				m_is_finished = true;
			}
		}
	}
}

void SpriteAnimation::UpdateConstantBuffer()
{
	if (m_p_owner_animator2D == nullptr || m_animation_frame_vector.empty())
		return;

	Vector2 atlas_texture_size = Vector2(static_cast<float>(m_p_atlas_texture->GetWidth()), static_cast<float>(m_p_atlas_texture->GetHeight()));

	auto animation_frame = m_animation_frame_vector[m_current_frame_id];

	animation_frame.left_top = Vector2(animation_frame.left_top.x / atlas_texture_size.x, animation_frame.left_top.y / atlas_texture_size.y);
	animation_frame.frame_size = Vector2(animation_frame.frame_size.x / atlas_texture_size.x, animation_frame.frame_size.y / atlas_texture_size.y);

	g_cbuffer_sprite_animation.sprite_animation_frame = animation_frame;

	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::SpriteAnimation);
	constant_buffer->SetConstantBufferData(&g_cbuffer_sprite_animation, sizeof(CBuffer_SpriteAnimation));
	constant_buffer->SetBufferBindStage(PipelineStage::PS); //애니메이션 픽셀계산은 Pixel Shader에서만 수행
	constant_buffer->BindPipeline();
}

void SpriteAnimation::Play()
{
	m_is_paused = false;
	m_is_finished = false;

	//정방향 재생인 경우
	if (!m_p_owner_animator2D->GetIsPlayReverse())
		m_current_frame_id = 0;

	//역방향 재생인 경우
	else
		m_current_frame_id = (static_cast<int>(m_animation_frame_vector.size()) - 1);
}

void SpriteAnimation::Pause()
{
	m_is_paused = true;
}

void SpriteAnimation::Stop()
{
	m_is_paused = false;
	m_is_finished = false;

	m_accumulate_time = 0.0f;
	//정방향 재생인 경우
	if (!m_p_owner_animator2D->GetIsPlayReverse())
		m_current_frame_id = 0;

	//역방향 재생인 경우
	else
		m_current_frame_id = (static_cast<int>(m_animation_frame_vector.size()) - 1);

	ResetAnimationEventFlag();
}

void SpriteAnimation::SetAnimationEvent(const UINT& clip_index, std::function<void(void)> event_func)
{
	m_animation_event_func_multimap.insert(std::make_pair(clip_index, std::make_pair(false, event_func)));
}

void SpriteAnimation::DoAnimationEvent(const UINT& clip_index)
{
	for (auto iter = m_animation_event_func_multimap.lower_bound(clip_index); iter != m_animation_event_func_multimap.upper_bound(clip_index); ++iter)
	{
		auto animation_event = iter->second;
		//이미 수행한 경우
		if (animation_event.first)
			return;

		animation_event.first = true;
		animation_event.second();
	}
}

void SpriteAnimation::ResetAnimationEventFlag()
{
	for (auto& animation_event : m_animation_event_func_multimap)
	{
		animation_event.second.first = false;
	}
}

bool SpriteAnimation::SaveToFile(const std::string& animation2D_path)
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, animation2D_path.c_str(), "wb"); //파일 쓰기

	if (p_file != nullptr)
	{
		//Sprite Animation Name
		fprintf(p_file, "[Sprite Animation Name]\n");
		fprintf(p_file, "%s\n", m_object_name.c_str());

		//Atlas Texture
		fprintf(p_file, "[Atlas Texture]\n");
		RESOURCE_MANAGER->SaveResource<Texture>(m_p_atlas_texture, p_file);

		//Sprite Animation Frame Count
		fprintf(p_file, "[Sprite Animation Frame Count]\n");
		fprintf(p_file, "%d\n", m_animation_frame_vector.size());

		//Sprite Animation Frame
		fprintf(p_file, "[Sprite Animation Frame]\n");
		for (UINT i = 0; i < m_animation_frame_vector.size(); ++i)
		{
			auto animation2D_frame = m_animation_frame_vector[i];
		
			fprintf(p_file, "[Index]\n");
			fprintf(p_file, "%d\n", i);
			fprintf(p_file, "[Left Top]\n");
			FILE_MANAGER->FPrintf_Vector2(animation2D_frame.left_top, p_file);
			fprintf(p_file, "[Frame Size]\n");
			FILE_MANAGER->FPrintf_Vector2(animation2D_frame.frame_size, p_file);
			fprintf(p_file, "[Duration]\n");
			fprintf(p_file, "%f\n", animation2D_frame.duration);
		}

		fclose(p_file);

		return true;
	}

	else
		return false;
}

bool SpriteAnimation::LoadFromFile(const std::string& animation2D_path)
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, animation2D_path.c_str(), "rb"); //파일 읽기

	if (p_file != nullptr)
	{
		char char_buffer[256] = {};

		//Sprite Animation Name
		FILE_MANAGER->FScanf(char_buffer, p_file);
		FILE_MANAGER->FScanf(char_buffer, p_file);
		m_object_name = std::string(char_buffer);

		//Atlas Texture
		FILE_MANAGER->FScanf(char_buffer, p_file);
		RESOURCE_MANAGER->LoadResource<Texture>(m_p_atlas_texture, p_file);

		//Sprite Animation Frame Count
		FILE_MANAGER->FScanf(char_buffer, p_file);
		UINT animation2D_frame_count = 0;
		fscanf_s(p_file, "%d\n", &animation2D_frame_count);
		m_animation_frame_vector.reserve(animation2D_frame_count);

		//Sprite Animation Frame
		FILE_MANAGER->FScanf(char_buffer, p_file);
		for (UINT i = 0; i < animation2D_frame_count; ++i)
		{
			SpriteAnimation_Frame animation2D_frame;
			ZeroMemory(&animation2D_frame, sizeof(SpriteAnimation_Frame));

			//Index
			FILE_MANAGER->FScanf(char_buffer, p_file);
			FILE_MANAGER->FScanf(char_buffer, p_file);

			//Left Top
			FILE_MANAGER->FScanf(char_buffer, p_file);
			FILE_MANAGER->FScanf_Vector2(animation2D_frame.left_top, p_file);

			//Frame Size
			FILE_MANAGER->FScanf(char_buffer, p_file);
			FILE_MANAGER->FScanf_Vector2(animation2D_frame.frame_size, p_file);

			//Duration
			FILE_MANAGER->FScanf(char_buffer, p_file);
			fscanf_s(p_file, "%f\n", &(animation2D_frame.duration));

			//Add Sprite Animation Frame
			m_animation_frame_vector.emplace_back(animation2D_frame);
		}

		fclose(p_file);

		return true;
	}

	else
		return false;
}
