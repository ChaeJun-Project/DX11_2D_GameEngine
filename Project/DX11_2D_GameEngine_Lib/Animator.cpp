#include "stdafx.h"
#include "Animator.h"

#include "Animation.h"

Animator::Animator()
	:IComponent(ComponentType::Animator)
{

}

Animator::Animator(const Animator& origin)
	: IComponent(origin.GetComponentType())
{
	//동일한 애니메이션의 정보를 바탕으로 새로 애니메이션 생성 및 추가
	for (auto& animation_iter : origin.m_p_animation_map)
	{
		auto animation = animation_iter.second;

		auto copy_animation = std::make_shared<Animation>(*animation.get());
		auto copy_animation_iter = this->m_p_animation_map.insert(std::make_pair(copy_animation->GetResourceName(), copy_animation));
		auto result = copy_animation_iter.second;
		assert(result);
	}

	this->m_p_current_animation = GetAnimation(origin.m_p_current_animation->GetResourceName());
}

Animator::~Animator()
{
	m_p_current_animation.reset();

	for (auto& animation_iter : this->m_p_animation_map)
	{
		animation_iter.second.reset();
	}
	this->m_p_animation_map.clear();
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

	auto renderer = m_p_owner_game_object->GetComponent<Renderer>();
	renderer->SetMesh(this->m_p_current_animation->GetMesh());
	auto material = renderer->GetMaterial();
	material->SetShader(ResourceManager::GetInstance()->GetShaderResource(ShaderResourceType::Standard));
	material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, this->m_p_current_animation->GetCurrentTexture());

	auto collider2D = m_p_owner_game_object->GetComponent<Renderer>();
	collider2D->SetMesh(this->m_p_current_animation->GetMesh());
}

void Animator::Play(const bool& is_play_reverse)
{
	if (this->m_p_current_animation == nullptr)
		return;

	//정방향 재생
	if (!is_play_reverse)
		this->m_p_current_animation->Play();

	//역재생
	else
		this->m_p_current_animation->PlayReverse();
}

void Animator::CreateAnimation(const std::string& animation_name, const std::string& animation_directory_path, const float& animation_playtime, const bool& is_loop)
{
	auto animation = std::make_shared<Animation>(animation_name);
	animation->SetAnimationTime(animation_playtime);
	animation->SetIsLoop(is_loop);
	animation->LoadFromFile(animation_directory_path);

	auto animation_iter = this->m_p_animation_map.insert(std::make_pair(animation_name, animation));
	auto result = animation_iter.second;
	assert(result);
}

void Animator::LoadAnimation(const std::string& animation_file_path)
{
}

const std::shared_ptr<Animation>& Animator::GetAnimation(const std::string& animation_name)
{
	auto animation_iter = this->m_p_animation_map.find(animation_name);

	//해당 사이즈의 메쉬를 찾지 못했을 경우
	if (animation_iter == this->m_p_animation_map.end())
		return nullptr;

	return animation_iter->second;
}

void Animator::SetCurrentAnimation(const std::string& animation_name)
{
	if (this->m_p_current_animation != nullptr)
	{
		if (this->m_p_current_animation->GetResourceName() == animation_name)
			return;

		else
			this->m_p_current_animation->Stop();
	}

	auto animation_iter = this->m_p_animation_map.find(animation_name);
	//해당 애니메이션을 찾지 못했다면
	if (animation_iter == this->m_p_animation_map.end())
		return;

	this->m_p_current_animation = animation_iter->second;

	Play();
}

void Animator::SetAnimationEvent(const std::string& animation_name, const UINT& clip_index, std::function<void(void)> event_func)
{
	auto animation = GetAnimation(animation_name);
	if (animation == nullptr)
		return;

	animation->SetAnimationEvent(clip_index, event_func);
}