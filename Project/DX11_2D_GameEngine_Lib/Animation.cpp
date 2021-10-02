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
		//Ȯ���ڰ� ���Ե� ���� �̸����� Ȯ���� �κ��� �� string�� �ش� �ؽ�ó�� �̸����� ����
		Texture texture(FileManager::GetIntactFileNameFromPath(file_name));
		texture.SetPipelineStage(TexturePipelineStage::PS, 0);
		auto result = texture.LoadFromFile(animation_path + file_name);
		assert(result);
		if (!result)
			return false;

		this->m_texture_vector.emplace_back(texture);
	}

	//Create Mesh
	//�ִϸ��̼��� �̸��� Mesh �̸����� ����
	this->m_p_mesh = new Mesh<VertexColorTexture>(FileManager::GetIntactFileNameFromPath(animation_path));
	this->m_p_mesh->Create(MeshType::Rectangle, m_texture_vector[0].GetTextureSize());

	//�ִϸ��̼� �����Ӵ� ���� �ð� ����
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

	//�ð� ����
	this->m_accumulate_time += TimeManager::GetInstance()->GetDeltaTime_float();

	//�ش� �������� ���� �ð��� ����� ���
	if (this->m_animation_frame_duration <= this->m_accumulate_time)
	{
		this->m_accumulate_time = 0.0f;

		//�ִϸ��̼� ������� ���
		if (this->m_is_play_reverse)
		{
			--this->m_current_frame_id;
			//�ִϸ��̼� ������� ������ ��
			if (this->m_current_frame_id < 0)
			{
				this->m_current_frame_id = static_cast<int>(this->m_texture_vector.size());

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
			if (this->m_current_frame_id > this->m_texture_vector.size())
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
