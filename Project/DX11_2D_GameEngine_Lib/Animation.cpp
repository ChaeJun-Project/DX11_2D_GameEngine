#include "stdafx.h"
#include "Animation.h"
#include "Texture.h"
#include "Mesh.h"

#include "ResourceManager.h"

Animation::Animation(const std::string resource_name)
	:IResource(ResourceType::Animation, resource_name)
{
}

Animation::Animation(const Animation& origin)
	: IResource(ResourceType::Animation, origin.GetResourceName())
{
	//Path
	m_resource_path = origin.GetResourcePath();
	//Copy Texture Vector
	std::copy
	(
		origin.m_p_texture_vector.begin(),
		origin.m_p_texture_vector.end(),
		std::inserter(m_p_texture_vector, m_p_texture_vector.end())
	);
	//Mesh
	m_p_mesh = origin.m_p_mesh;
	//Copy Animation Event Vector
	std::copy
	(
		origin.m_animation_event_func_map.begin(),
		origin.m_animation_event_func_map.end(),
		std::inserter(m_animation_event_func_map, m_animation_event_func_map.end())
	);

	m_animation_speed = origin.m_animation_speed;
	m_animation_time = origin.m_animation_time;
	m_animation_speed = origin.m_animation_speed;
	m_is_play_reverse = origin.m_is_play_reverse;
	m_is_loop = origin.m_is_loop;
}

Animation::~Animation()
{
	//Delete All Texture Vector
	m_p_texture_vector.clear();
	m_p_texture_vector.shrink_to_fit();

	//Delete All Animation Event Function Map
	m_animation_event_func_map.clear();
}

//Texture/objectname/~~/
void Animation::LoadFromFile(const std::string& animation_directory_path)
{
	m_resource_path = animation_directory_path;

	auto resource_manager = ResourceManager::GetInstance();

	auto file_name_vector = FileManager::GetFileNameVectorFromDirectory(animation_directory_path);

	m_p_texture_vector.reserve(file_name_vector.size());

	//Create Texture & Push Vector
	for (auto& file_name : file_name_vector)
	{
		m_p_texture_vector.emplace_back(resource_manager->CreateTexture(animation_directory_path + file_name));
	}

	//Create Mesh
	m_p_mesh = resource_manager->GetResource<Mesh>("Rectangle_Mesh");
}

void Animation::SaveFile(const std::string& animation_directory_path)
{
}

void Animation::Update()
{
	if (!m_is_playing || m_animation_frame_duration == 0.0f)
		return;

	if (m_is_end)
		m_is_end = false;

	//�ð� ����
	m_accumulate_time += (TimeManager::GetInstance()->GetDeltaTime_float() * m_animation_speed);

	//�ش� �������� ���� �ð��� ����� ���
	if (m_animation_frame_duration <= m_accumulate_time)
	{
		m_accumulate_time = 0.0f;

		//�ִϸ��̼� ������� ���
		if (m_is_play_reverse)
		{
			--m_current_frame_id;
			//�ִϸ��̼� ������� ������ ��
			if (m_current_frame_id <= 0)
			{
				m_current_frame_id = static_cast<int>(m_p_texture_vector.size());

				//�ݺ� ����� �ƴ� ���
				if (!m_is_loop)
					m_is_playing = false; //��� ����

				//�ش� �ִϸ��̼��� ������ 1ȸ ����Ǿ����� �˷���
				m_is_end = true;
			}
		}

		//�ִϸ��̼��� ������ ����� ���
		else
		{
			++m_current_frame_id;
			//�ִϸ��̼� ����� ������ ��
			if (m_current_frame_id >= m_p_texture_vector.size())
			{
				m_current_frame_id = 0;

				//�ݺ� ����� �ƴ� ���
				if (!m_is_loop)
					m_is_playing = false;//��� ����

				//�ش� �ִϸ��̼��� ������ 1ȸ ����Ǿ����� �˷���
				m_is_end = true;
			}
		}

		DoAnimationEvent(static_cast<UINT>(m_current_frame_id));
	}
}

void Animation::Play()
{
	//�ִϸ��̼� �����Ӵ� ���� �ð� ����
	m_animation_frame_duration = static_cast<float>(m_animation_time / m_p_texture_vector.size());
	m_is_playing = true;
	m_is_end = false;
	m_current_frame_id = 0;
}

void Animation::PlayReverse()
{
	//�ִϸ��̼� �����Ӵ� ���� �ð� ����
	m_animation_frame_duration = static_cast<float>(m_animation_time / m_p_texture_vector.size());
	m_is_play_reverse = true;
	m_is_end = false;
	m_current_frame_id = static_cast<int>(m_p_texture_vector.size());
}

void Animation::Stop()
{
	m_is_playing = false;
	m_is_end = false;
	m_current_frame_id = 0;
	ResetAnimationEventFlag();
}

void Animation::SetAnimationEvent(const UINT& clip_index, std::function<void(void)> event_func)
{
	auto map_iter = m_animation_event_func_map.insert(std::make_pair(clip_index, std::make_pair(false, event_func)));
	auto result = map_iter.second;
	if (!result)
		return;
}

void Animation::DoAnimationEvent(const UINT& clip_index)
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

void Animation::ResetAnimationEventFlag()
{
	for (auto& animation_event : m_animation_event_func_map)
	{
		animation_event.second.first = false;
	}
}

