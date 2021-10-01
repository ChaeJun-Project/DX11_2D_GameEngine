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

	return true;
}

void Animation::Update()
{
	if (!this->m_is_playing || this->m_animation_frame == 0.0f || this->m_animation_speed == 0.0f)
		return;

    //애니메이션 프레임 전환 다시 생각해보기
    auto frame_update_time = static_cast<double>(this->m_animation_time/ this->m_texture_vector.size());

	if (this->m_is_loop)
	{

	}
}

void Animation::Render()
{
	if (this->m_is_playing)
	{
		this->m_texture_vector[this->m_current_clip_id].BindPipeline();
		this->m_p_mesh->BindPipeline();
	}
}

void Animation::Play()
{
	this->m_is_playing = true;
}

void Animation::Stop()
{
	this->m_is_playing = false;
	this->m_current_clip_id = 0;
}

void Animation::PlayReverse()
{
}

void Animation::SetAnimationEvent(const UINT& motion_index, std::function<void(void)> event_func)
{
}
