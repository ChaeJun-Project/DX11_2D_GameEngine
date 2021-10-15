#include "stdafx.h"
#include "Animator.h"

Animator::Animator()
	:IComponent(ComponentType::Animator)
{
	
}

void Animator::Update()
{
	if (this->m_p_current_animation == nullptr)
		return;

	this->m_p_current_animation->Update();
}

void Animator::FinalUpdate()
{
	if (this->m_p_current_animation == nullptr)
		return;

	auto renderer = m_p_owner_game_object.lock()->GetComponent<Renderer>();
	renderer->SetMesh(this->m_p_current_animation->GetMesh());
	auto material = renderer->GetMaterial();
	material->SetShader(ResourceManager::GetInstance()->GetShaderResource(ShaderResourceType::Standard));
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, this->m_p_current_animation->GetCurrentTexture());
}

void Animator::Play(const bool& is_play_reverse)
{
	if (this->m_p_current_animation == nullptr)
	  return;
	
	//정방향 재생
	if(!is_play_reverse)
		this->m_p_current_animation->Play();

	//역재생
	else
		this->m_p_current_animation->PlayReverse();
}

void Animator::CreateAnimation(const std::string& animation_name, const std::string& animation_directory_path, const float& animation_loop_time)
{
    auto animation = std::make_shared<Animation>(animation_name);
	animation->SetAnimationTime(animation_loop_time);
	animation->SetIsLoop(true);
	animation->LoadFromFile(animation_directory_path);

	auto animation_iter = this->m_p_animation_map.insert(std::make_pair(animation_name, animation));
	auto result = animation_iter.second;
	assert(result);
}

void Animator::LoadAnimation(const std::string& animation_file_path)
{
}

void Animator::GetAnimation(const std::string& animation_name)
{
}


void Animator::SetCurrentAnimation(const std::string& animation_name)
{
	if (this->m_p_current_animation != nullptr)
		this->m_p_current_animation->Stop();

	auto animation_iter = this->m_p_animation_map.find(animation_name);
	//해당 애니메이션을 찾지 못했다면
	if (animation_iter == this->m_p_animation_map.end())
		return;

	this->m_p_current_animation = animation_iter->second;
}

void Animator::SetAnimationEvent(const std::string& animation_name, std::function<void(void)> func)
{
	//TODO
}