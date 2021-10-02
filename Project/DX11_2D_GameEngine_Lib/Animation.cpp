#include "stdafx.h"
#include "Animation.h"
#include "Texture.h"
#include "Mesh.h"

Animation::Animation(const std::string resource_name)
	:IResource(ResourceType::Animation, resource_name)
{
}

Animation::~Animation()
{
	//Delete All Texture Vector
	this->m_texture_vector.clear();
	this->m_texture_vector.shrink_to_fit();

	//Delete All Animation Event Function Map
	this->m_animation_event_func_map.clear();
}

const bool& Animation::LoadFromFile(const std::string& animation_path)
{
	auto file_name_vector = FileManager::GetFileNameVectorFromDirectory(animation_path);

	this->m_texture_vector.reserve(file_name_vector.size());

	//Create Texture & Push Vector
	for (auto& file_name : file_name_vector)
	{
		//확장자가 포함된 파일 이름에서 확장자 부분을 뺀 string을 해당 텍스처의 이름으로 설정
		Texture texture(FileManager::GetIntactFileNameFromPath(file_name));
		texture.SetPipelineStage(TexturePipelineStage::PS, 0);
		auto result = texture.LoadFromFile(animation_path + file_name);
		assert(result);
		if (!result)
			return false;

		this->m_texture_vector.emplace_back(texture);
	}

	//Create Mesh
	//애니메이션의 이름을 Mesh 이름으로 설정
	this->m_p_mesh = new Mesh<VertexColorTexture>(FileManager::GetIntactFileNameFromPath(animation_path));
	this->m_p_mesh->Create(MeshType::Rectangle, m_texture_vector[0].GetTextureSize());

	//애니메이션 프레임당 유지 시간 설정
	this->m_animation_frame_duration = static_cast<float>(this->m_animation_time / this->m_texture_vector.size());


	return true;
}

void Animation::SaveFile(const std::string& animation_path)
{
}

void Animation::Update()
{
	if (!this->m_is_playing || this->m_animation_frame_duration == 0.0f)
		return;

	if (this->m_is_end)
		this->m_is_end = false;

	//시간 누적
	this->m_accumulate_time += TimeManager::GetInstance()->GetDeltaTime_float();

	//해당 프레임의 유지 시간이 경과한 경우
	if (this->m_animation_frame_duration <= this->m_accumulate_time)
	{
		this->m_accumulate_time = 0.0f;

		//애니메이션 역재생인 경우
		if (this->m_is_play_reverse)
		{
			--this->m_current_frame_id;
			//애니메이션 역재생이 끝났을 때
			if (this->m_current_frame_id < 0)
			{
				this->m_current_frame_id = static_cast<int>(this->m_texture_vector.size());

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
			if (this->m_current_frame_id > this->m_texture_vector.size())
			{
				this->m_current_frame_id = 0;

				//반복 재생이 아닌 경우
				if (!this->m_is_loop)
					this->m_is_playing = false;//재생 종료

				//해당 애니메이션이 끝까지 1회 재생되었음을 알려줌
				this->m_is_end = true;
			}
		}
	}
}

void Animation::Render()
{
	this->m_texture_vector[this->m_current_frame_id].BindPipeline();
	this->m_p_mesh->BindPipeline();
}

void Animation::Play()
{
	this->m_is_playing = true;
	this->m_is_end = false;
	this->m_current_frame_id = 0;
}

void Animation::PlayReverse()
{
	this->m_is_play_reverse = true;
	this->m_is_end = false;
	this->m_current_frame_id = static_cast<int>(this->m_texture_vector.size());
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
