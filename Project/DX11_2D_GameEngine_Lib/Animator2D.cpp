#include "stdafx.h"
#include "Animator2D.h"

#include "Animation2D.h"

Animator2D::Animator2D()
:IComponent(ComponentType::Animator2D)
{
}

Animator2D::Animator2D(const Animator2D& origin)
	: IComponent(origin.GetComponentType())
{
	//동일한 애니메이션의 정보를 바탕으로 새로 애니메이션 생성 및 추가
	for (auto& animation_iter : origin.m_p_animation_map)
	{
		auto animation = animation_iter.second;

		auto copy_animation = std::make_shared<Animation2D>(*animation.get());
		auto copy_animation_iter = m_p_animation_map.insert(std::make_pair(copy_animation->m_animation_name, copy_animation));
		auto result = copy_animation_iter.second;
		assert(result);
	}

	m_p_current_animation = GetAnimation(origin.m_p_current_animation->m_animation_name);
}

Animator2D::~Animator2D()
{
	m_p_current_animation.reset();

	for (auto& animation_iter : m_p_animation_map)
	{
		animation_iter.second.reset();
	}
	m_p_animation_map.clear();
}

void Animator2D::Start()
{
	if (m_p_current_animation == nullptr)
		return;

	m_p_current_animation->m_current_frame_id = 0;

	m_is_playing = true;

	m_p_current_animation->Play();
}

void Animator2D::Update()
{
	if (m_p_current_animation == nullptr)
		return;

	m_p_current_animation->Update();
}

void Animator2D::FinalUpdate()
{
	if (m_p_current_animation == nullptr)
		return;

	m_p_current_animation->FinalUpdate();

	//현재 애니메이션이 끝났고 반복 재생 옵션이 켜있는 경우
	if (m_p_current_animation->m_is_finished && m_is_loop)
	{
		m_p_current_animation->Play();
	}
}

void Animator2D::Play()
{
	if (m_p_current_animation == nullptr)
		return;

	m_is_playing = true;

	m_p_current_animation->Play();
}

void Animator2D::Pause()
{
	if (m_p_current_animation == nullptr)
		return;

	m_is_playing = false;

	m_p_current_animation->Pause();
}

void Animator2D::Stop()
{
	if (m_p_current_animation == nullptr)
		return;

	m_is_playing = false;

	m_p_current_animation->Stop();
}

const std::shared_ptr<Animation2D>& Animator2D::CreateAnimation(const std::string& animation_name, const std::shared_ptr<Texture>& p_atlas_texture)
{
	auto animation2D = std::make_shared<Animation2D>();
	animation2D->m_animation_name = animation_name;
	animation2D->m_p_owner_animator2D = this;
	animation2D->m_p_atlas_texture = p_atlas_texture;

	auto animation_iter = m_p_animation_map.insert(std::make_pair(animation_name, animation2D));
	auto result = animation_iter.second;
	assert(result);
	if(!result)
	  return nullptr;

	return animation_iter.first->second;
}

const std::shared_ptr<Animation2D>& Animator2D::GetAnimation(const std::string& animation_name)
{
	auto animation_iter = m_p_animation_map.find(animation_name);

	//해당 사이즈의 메쉬를 찾지 못했을 경우
	if (animation_iter == m_p_animation_map.end())
		return nullptr;

	return animation_iter->second;
}

void Animator2D::SetCurrentAnimation(const std::string& animation_name)
{
	if (m_p_current_animation != nullptr)
	{
		if (m_p_current_animation->m_animation_name == animation_name)
			return;

		else
			m_p_current_animation->Stop();
	}

	auto animation_iter = m_p_animation_map.find(animation_name);
	//해당 애니메이션을 찾지 못했다면
	if (animation_iter == m_p_animation_map.end())
		return;

	m_p_current_animation = animation_iter->second;

	Play();
}

const std::shared_ptr<Texture>& Animator2D::GetAtlasTexture()
{
	if (m_p_current_animation)
		return m_p_current_animation->m_p_atlas_texture;

	return nullptr;
}

void Animator2D::SetAnimationEvent(const std::string& animation_name, const UINT& clip_index, std::function<void(void)> event_func)
{
auto animation = GetAnimation(animation_name);
	if (animation == nullptr)
		return;

	animation->SetAnimationEvent(clip_index, event_func);
}

void Animator2D::SaveToScene(FILE* p_file)
{
}

void Animator2D::LoadFromScene(FILE* p_file)
{
}
