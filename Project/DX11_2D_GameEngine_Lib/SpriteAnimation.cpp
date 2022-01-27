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
	: IResource(origin.m_resource_type, origin.m_resource_name)
{
	m_resource_path = origin.m_resource_path;

	auto animation_frame_count = origin.m_animation_frame_vector.size();

	m_animation_frame_vector.reserve(animation_frame_count);

	for (const auto& animation_frame : origin.m_animation_frame_vector)
	{
		m_animation_frame_vector.emplace_back(animation_frame);
	}

	m_p_atlas_texture = origin.m_p_atlas_texture;
}

SpriteAnimation::~SpriteAnimation()
{
	m_p_owner_animator2D = nullptr;

	m_animation_frame_vector.clear();
	m_animation_frame_vector.shrink_to_fit();

	m_p_atlas_texture.reset();
}

void SpriteAnimation::Update()
{
	if (m_p_owner_animator2D == nullptr || m_animation_frame_vector.empty() || m_is_paused || m_is_finished)
		return;

	auto animation_speed = m_p_owner_animator2D->GetAnimationSpeed();
	auto delta_time_f = DELTA_TIME_F;
	//�ð� ����
	m_accumulate_time += (delta_time_f * animation_speed);

	//�ش� �������� ���� �ð��� ����� ���
	if (m_animation_frame_vector[m_current_frame_id].duration <= m_accumulate_time)
	{
		m_accumulate_time = 0.0f;

		//�ִϸ��̼� ������� ���
		if (m_p_owner_animator2D->GetIsPlayReverse())
		{
			--m_current_frame_id;
			//�ִϸ��̼� ������� ������ ��
			if (m_current_frame_id <= 0)
			{
				m_current_frame_id = static_cast<int>(m_animation_frame_vector.size());

				//�ش� �ִϸ��̼��� ������ 1ȸ ����Ǿ����� �˷���
				m_is_finished = true;
			}
		}

		//�ִϸ��̼��� ������ ����� ���
		else
		{
			++m_current_frame_id;
			//�ִϸ��̼� ����� ������ ��
			if (m_current_frame_id >= m_animation_frame_vector.size())
			{
				m_current_frame_id = 0;

				//�ش� �ִϸ��̼��� ������ 1ȸ ����Ǿ����� �˷���
				m_is_finished = true;
			}
		}
		
		DoAnimationEvent(static_cast<UINT>(m_current_frame_id));
	}
}

void SpriteAnimation::FinalUpdate()
{
	if (m_p_owner_animator2D == nullptr || m_animation_frame_vector.empty())
		return;

	auto animator2D_index = m_p_owner_animator2D->GetAnimator2DID();

	SpriteAnimation_Data animation2D_data = m_animation_frame_vector[m_current_frame_id].animation2D_data;

	Vector2 atlas_texture_size = Vector2(static_cast<float>(m_p_atlas_texture->GetWidth()), static_cast<float>(m_p_atlas_texture->GetHeight()));

	animation2D_data.left_top = Vector2(animation2D_data.left_top.x / atlas_texture_size.x, animation2D_data.left_top.y / atlas_texture_size.y);
	animation2D_data.frame_size = Vector2(animation2D_data.frame_size.x / atlas_texture_size.x, animation2D_data.frame_size.y / atlas_texture_size.y);
	animation2D_data.full_frame_size = Vector2(animation2D_data.full_frame_size.x / atlas_texture_size.x, animation2D_data.full_frame_size.y / atlas_texture_size.y);

	g_cbuffer_animation2D.animation2D_data_array[animator2D_index] = animation2D_data;

	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::SpriteAnimation);
	constant_buffer->SetConstantBufferData(&g_cbuffer_animation2D, sizeof(CBuffer_SpriteAnimation));
	constant_buffer->SetBufferBindStage(PipelineStage::PS); //�ִϸ��̼� �ȼ������ Pixel Shader������ ����
	constant_buffer->BindPipeline();
}

void SpriteAnimation::Play()
{
	m_is_paused = false;
	m_is_finished = false;

	//������ ����� ���
	if (!m_p_owner_animator2D->GetIsPlayReverse())
		m_current_frame_id = 0;

	//������ ����� ���
	else
		m_current_frame_id = static_cast<int>(m_animation_frame_vector.size());
}

void SpriteAnimation::Pause()
{
	m_is_paused = true;
}

void SpriteAnimation::Stop()
{
	m_is_paused = false;
	m_is_finished = false;

	m_current_frame_id = 0;
	ResetAnimationEventFlag();
}

void SpriteAnimation::SetAnimationEvent(const UINT& clip_index, std::function<void(void)> event_func)
{
	auto map_iter = m_animation_event_func_map.insert(std::make_pair(clip_index, std::make_pair(false, event_func)));
	auto result = map_iter.second;
	if (!result)
		return;
}

void SpriteAnimation::DoAnimationEvent(const UINT& clip_index)
{
	auto animation_event_iter = m_animation_event_func_map.find(clip_index);
	if (animation_event_iter != m_animation_event_func_map.end())
	{
		auto animation_event = animation_event_iter->second;
		//�̹� ������ ���
		if (animation_event.first)
			return;

		animation_event.first = true;
		animation_event.second();
	}
}

void SpriteAnimation::ResetAnimationEventFlag()
{
	for (auto& animation_event : m_animation_event_func_map)
	{
		animation_event.second.first = false;
	}
}

bool SpriteAnimation::SaveToFile(const std::string& animation2D_path)
{
	auto resource_manager = ResourceManager::GetInstance();

	FILE* p_file = nullptr;
	fopen_s(&p_file, animation2D_path.c_str(), "wb"); //���� ����

	if (p_file != nullptr)
	{
		//Sprite Animation Name
		fprintf(p_file, "[Sprite Animation Name]\n");
		fprintf(p_file, "%s\n", m_resource_name.c_str());

		//Atlas Texture
		fprintf(p_file, "[Atlas Texture]\n");
		resource_manager->SaveResource<Texture>(m_p_atlas_texture, p_file);

		//Sprite Animation Frame Count
		fprintf(p_file, "[Sprite Animation Frame Count]\n");
		fprintf(p_file, "%d\n", m_animation_frame_vector.size());

		//Sprite Animation Frame
		fprintf(p_file, "[Sprite Animation Frame]\n");
		for (UINT i = 0; i < m_animation_frame_vector.size(); ++i)
		{
			auto animation2D_frame = m_animation_frame_vector[i];
			auto animation2D_frame_data = animation2D_frame.animation2D_data;

			fprintf(p_file, "[Index]\n");
			fprintf(p_file, "%d\n", i);
			fprintf(p_file, "[Left Top]\n");
			FILE_MANAGER->FPrintf_Vector2(animation2D_frame_data.left_top, p_file);
			fprintf(p_file, "[Frame Size]\n");
			FILE_MANAGER->FPrintf_Vector2(animation2D_frame_data.frame_size, p_file);
			fprintf(p_file, "[Full Frame Size]\n");
			FILE_MANAGER->FPrintf_Vector2(animation2D_frame_data.full_frame_size, p_file);
			fprintf(p_file, "[Offset]\n");
			FILE_MANAGER->FPrintf_Vector2(animation2D_frame_data.offset, p_file);
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
	auto resource_manager = ResourceManager::GetInstance();

	FILE* p_file = nullptr;
	fopen_s(&p_file, animation2D_path.c_str(), "rb"); //���� �б�

	if (p_file != nullptr)
	{
		char char_buffer[256] = {};

		//Sprite Animation Name
		FILE_MANAGER->FScanf(char_buffer, p_file);
		FILE_MANAGER->FScanf(char_buffer, p_file);
		m_resource_name = std::string(char_buffer);

		//Atlas Texture
		FILE_MANAGER->FScanf(char_buffer, p_file);
		resource_manager->LoadResource<Texture>(m_p_atlas_texture, p_file);

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
			FILE_MANAGER->FScanf_Vector2(animation2D_frame.animation2D_data.left_top, p_file);

			//Frame Size
			FILE_MANAGER->FScanf(char_buffer, p_file);
			FILE_MANAGER->FScanf_Vector2(animation2D_frame.animation2D_data.frame_size, p_file);

			//Full Frame Size
			FILE_MANAGER->FScanf(char_buffer, p_file);
			FILE_MANAGER->FScanf_Vector2(animation2D_frame.animation2D_data.full_frame_size, p_file);

			//Offset
			FILE_MANAGER->FScanf(char_buffer, p_file);
			FILE_MANAGER->FScanf_Vector2(animation2D_frame.animation2D_data.offset, p_file);

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
