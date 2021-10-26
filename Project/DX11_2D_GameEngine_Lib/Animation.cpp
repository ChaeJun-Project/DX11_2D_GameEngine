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

	//시간 누적
	this->m_accumulate_time += (TimeManager::GetInstance()->GetDeltaTime_float() * this->m_animation_speed);

	//해당 프레임의 유지 시간이 경과한 경우
	if (this->m_animation_frame_duration <= this->m_accumulate_time)
	{
		this->m_accumulate_time = 0.0f;

		//애니메이션 역재생인 경우
		if (this->m_is_play_reverse)
		{
			--this->m_current_frame_id;
			//애니메이션 역재생이 끝났을 때
			if (this->m_current_frame_id <= 0)
			{
				this->m_current_frame_id = static_cast<int>(this->m_p_texture_vector.size());

				//반복 재생이 아닌 경우
				if (!this->m_is_loop)
					this->m_is_playing = false; //재생 종료

				//해당 애니메이션이 끝까지 1회 재생되었음을 알려줌
				this->m_is_end = true;
			}
		}

		//애니메이션이 정방향 재생인 경우
		else
		{
			++this->m_current_frame_id;
			//애니메이션 재생이 끝났을 때
			if (this->m_current_frame_id >= this->m_p_texture_vector.size())
			{
				this->m_current_frame_id = 0;

				//반복 재생이 아닌 경우
				if (!this->m_is_loop)
					this->m_is_playing = false;//재생 종료

				//해당 애니메이션이 끝까지 1회 재생되었음을 알려줌
				this->m_is_end = true;
			}
		}

		DoAnimationEvent(static_cast<UINT>(this->m_current_frame_id));
	}
}

void Animation::Play()
{
	//애니메이션 프레임당 유지 시간 설정
	this->m_animation_frame_duration = static_cast<float>(this->m_animation_time / this->m_p_texture_vector.size());
	this->m_is_playing = true;
	this->m_is_end = false;
	this->m_current_frame_id = 0;
}

void Animation::PlayReverse()
{
	//애니메이션 프레임당 유지 시간 설정
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
	ResetAnimationEventFlag();
}

void Animation::SetAnimationEvent(const UINT& clip_index, std::function<void(void)> event_func)
{
	auto map_iter = this->m_animation_event_func_map.insert(std::make_pair(clip_index, std::make_pair(false, event_func)));
	auto result = map_iter.second;
	if (!result)
		return;
}

void Animation::DoAnimationEvent(const UINT& clip_index)
{
	auto animation_event_iter = this->m_animation_event_func_map.find(clip_index);
	if (animation_event_iter != this->m_animation_event_func_map.end())
	{
		auto animation_event = animation_event_iter->second;
		//이미 수행한 경우
		if (animation_event.first)
			return;

		animation_event.first = true;
		animation_event.second();
	}
}

void Animation::ResetAnimationEventFlag()
{
	for (auto& animation_event : this->m_animation_event_func_map)
	{
		animation_event.second.first = false;
	}
}

