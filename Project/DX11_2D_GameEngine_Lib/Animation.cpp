#include "stdafx.h"
#include "Animation.h"
#include "Texture.h"
#include "Mesh.h"

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
		std::inserter(this->m_p_texture_vector, this->m_p_texture_vector.end())
	);
	//Mesh
	this->m_p_mesh = origin.m_p_mesh;
	//Copy Animation Event Vector
	std::copy
	(
		origin.m_animation_event_func_map.begin(),
		origin.m_animation_event_func_map.end(),
		std::inserter(this->m_animation_event_func_map, this->m_animation_event_func_map.end())
	);

	this->m_animation_speed = origin.m_animation_speed;
	this->m_animation_time = origin.m_animation_time;
	this->m_animation_speed = origin.m_animation_speed;
	this->m_is_play_reverse = origin.m_is_play_reverse;
	this->m_is_loop = origin.m_is_loop;
}

Animation::~Animation()
{
	//Delete All Texture Vector
	this->m_p_texture_vector.clear();
	this->m_p_texture_vector.shrink_to_fit();

	//Delete All Animation Event Function Map
	this->m_animation_event_func_map.clear();
}

//Texture/objectname/~~/
void Animation::LoadFromFile(const std::string& animation_directory_path)
{
	m_resource_path = animation_directory_path;

	auto resource_manager = ResourceManager::GetInstance();

	auto file_name_vector = FileManager::GetFileNameVectorFromDirectory(animation_directory_path);

	this->m_p_texture_vector.reserve(file_name_vector.size());

	//Create Texture & Push Vector
	for (auto& file_name : file_name_vector)
	{
		this->m_p_texture_vector.emplace_back(resource_manager->LoadTexture(animation_directory_path + file_name));
	}

	//Create Mesh
	this->m_p_mesh = resource_manager->CreateMesh(m_p_texture_vector[0]->GetTextureSize());
}

void Animation::SaveFile(const std::string& animation_directory_path)
{
}

void Animation::Update()
{
	if (!this->m_is_playing || this->m_animation_frame_duration == 0.0f)
		return;

	if (this->m_is_end)
		this->m_is_end = false;

	//�ð� ����
	this->m_accumulate_time += (TimeManager::GetInstance()->GetDeltaTime_float() * this->m_animation_speed);

	//�ش� �������� ���� �ð��� ����� ���
	if (this->m_animation_frame_duration <= this->m_accumulate_time)
	{
		this->m_accumulate_time = 0.0f;

		//�ִϸ��̼� ������� ���
		if (this->m_is_play_reverse)
		{
			--this->m_current_frame_id;
			//�ִϸ��̼� ������� ������ ��
			if (this->m_current_frame_id <= 0)
			{
				this->m_current_frame_id = static_cast<int>(this->m_p_texture_vector.size());

				//�ݺ� ����� �ƴ� ���
				if (!this->m_is_loop)
					this->m_is_playing = false; //��� ����

				//�ش� �ִϸ��̼��� ������ 1ȸ ����Ǿ����� �˷���
				this->m_is_end = true;
			}
		}

		//�ִϸ��̼��� ������ ����� ���
		else
		{
			++this->m_current_frame_id;
			//�ִϸ��̼� ����� ������ ��
			if (this->m_current_frame_id >= this->m_p_texture_vector.size())
			{
				this->m_current_frame_id = 0;

				//�ݺ� ����� �ƴ� ���
				if (!this->m_is_loop)
					this->m_is_playing = false;//��� ����

				//�ش� �ִϸ��̼��� ������ 1ȸ ����Ǿ����� �˷���
				this->m_is_end = true;
			}
		}
	}

	//�ش� �ִϸ��̼� �����ӿ� �̺�Ʈ �Լ��� ��ϵǾ� �ִٸ� �̺�Ʈ �Լ� ����
	auto component_iter = this->m_animation_event_func_map.find(this->m_current_frame_id);
	if (component_iter != this->m_animation_event_func_map.end())
		component_iter->second;
}

void Animation::Play()
{
	//�ִϸ��̼� �����Ӵ� ���� �ð� ����
	this->m_animation_frame_duration = static_cast<float>(this->m_animation_time / this->m_p_texture_vector.size());
	this->m_is_playing = true;
	this->m_is_end = false;
	this->m_current_frame_id = 0;
}

void Animation::PlayReverse()
{
	//�ִϸ��̼� �����Ӵ� ���� �ð� ����
	this->m_animation_frame_duration = static_cast<float>(this->m_animation_time / this->m_p_texture_vector.size());
	this->m_is_play_reverse = true;
	this->m_is_end = false;
	this->m_current_frame_id = static_cast<int>(this->m_p_texture_vector.size());
}

void Animation::Stop()
{
	this->m_is_playing = false;
	this->m_is_end = false;
	this->m_current_frame_id = 0;
}

void Animation::SetAnimationEvent(const UINT& motion_index, std::function<void(void)> event_func)
{
	//TODO
}
