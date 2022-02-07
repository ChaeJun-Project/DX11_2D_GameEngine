#include "stdafx.h"
#include "Animator2D.h"

#include "ResourceManager.h"
#include "SpriteAnimation.h"

Animator2D::Animator2D()
	:IComponent(ComponentType::Animator2D)
{
}

Animator2D::Animator2D(const Animator2D& origin)
	: IComponent(origin.GetComponentType())
{
	//동일한 애니메이션의 정보를 바탕으로 새로 애니메이션 생성 및 추가
	for (auto& animation_iter : origin.m_p_sprite_animation_map)
	{
		/*auto animation = animation_iter.second;

		auto copy_animation = std::make_shared<Animation2D>(*animation.get());
		auto copy_animation_iter = m_p_sprite_animation_map.insert(std::make_pair(copy_animation->m_resource_name, copy_animation));
		auto result = copy_animation_iter.second;
		assert(result);*/
	}

	//m_p_current_animation = GetAnimation(origin.m_p_current_animation->m_resource_name);
}

Animator2D::~Animator2D()
{
	m_p_current_animation.reset();

	for (auto& animation_iter : m_p_sprite_animation_map)
	{
		animation_iter.second.reset();
	}
	m_p_sprite_animation_map.clear();
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

const std::shared_ptr<Texture>& Animator2D::GetCurrentAnimationAtlasTexture()
{
	if (m_p_current_animation == nullptr)
		return nullptr;

	return m_p_current_animation->GetAtlasTexture();
}

void Animator2D::AddAnimation(const std::string& sprite_animation_name)
{
	auto p_sprtie_animation = ResourceManager::GetInstance()->GetResource<SpriteAnimation>(sprite_animation_name);

	if (p_sprtie_animation == nullptr)
		return;

	auto p_clone_sprtie_animation = std::shared_ptr<SpriteAnimation>(p_sprtie_animation->Clone());
	p_clone_sprtie_animation->m_p_owner_animator2D = this;

	m_p_sprite_animation_map.insert(std::make_pair(sprite_animation_name, p_clone_sprtie_animation));
}

void Animator2D::DeleteAnimation(const std::string& sprite_animation_name)
{
	auto animation_iter = m_p_sprite_animation_map.find(sprite_animation_name);

	if (animation_iter == m_p_sprite_animation_map.end())
		return;

	m_p_sprite_animation_map.erase(sprite_animation_name);
}

const bool Animator2D::HasAnimation(const std::string& sprite_animation_name)
{
	auto animation_iter = m_p_sprite_animation_map.find(sprite_animation_name);

	if (animation_iter == m_p_sprite_animation_map.end())
		return false;

	return true;
}

void Animator2D::SetCurrentAnimation(const std::string& animation_name)
{
	if (m_p_current_animation != nullptr)
	{
		if (m_p_current_animation->m_object_name == animation_name)
			return;

		else
			m_p_current_animation->Stop();
	}

	auto animation_iter = m_p_sprite_animation_map.find(animation_name);
	//해당 애니메이션을 찾지 못했다면
	if (animation_iter == m_p_sprite_animation_map.end())
		return;

	m_p_current_animation = animation_iter->second;

	Play();
}

void Animator2D::SetAnimationEvent(const std::string& animation_name, const UINT& clip_index, std::function<void(void)> event_func)
{
	auto animation_iter = m_p_sprite_animation_map.find(animation_name);

	//해당 Sprite Animation을 찾지 못한 경우
	if (animation_iter == m_p_sprite_animation_map.end())
		return;

	animation_iter->second->SetAnimationEvent(clip_index, event_func);
}

void Animator2D::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	auto resource_manager = ResourceManager::GetInstance();

	//Animation2D Map
	fprintf(p_file, "[Animation2D Map]\n");
	//Count
	fprintf(p_file, "[Count]\n");
	fprintf(p_file, "%d\n", m_p_sprite_animation_map.size());
	//Data
	fprintf(p_file, "[Data]\n");
	for (const auto& animation2D : m_p_sprite_animation_map)
	{
		//Key
		fprintf(p_file, "[Key]\n");
		fprintf(p_file, "%s\n", animation2D.first.c_str());
		//Save Sprite Animation Resource
		resource_manager->SaveResource<SpriteAnimation>(animation2D.second, p_file);
	}

	//Animation Speed
	fprintf(p_file, "[Animation Speed]\n");
	fprintf(p_file, "%f\n", m_animation_speed);
}

void Animator2D::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = { 0 };

	auto resource_manager = ResourceManager::GetInstance();

	//Animation2D Map
	FILE_MANAGER->FScanf(char_buffer, p_file);
	//Count
	FILE_MANAGER->FScanf(char_buffer, p_file);
	UINT animation_count = 0;
	fscanf_s(p_file, "%d\n", &animation_count);
	//Data
	FILE_MANAGER->FScanf(char_buffer, p_file);
	for (UINT i = 0; i < animation_count; ++i)
	{
		//Key
		FILE_MANAGER->FScanf(char_buffer, p_file);
		FILE_MANAGER->FScanf(char_buffer, p_file);
		std::string animation_key = std::string(char_buffer);

		//Load Sprite Animation Resource
		std::shared_ptr<SpriteAnimation> p_animation2D = nullptr;
		resource_manager->LoadResource<SpriteAnimation>(p_animation2D, p_file);
		m_p_sprite_animation_map.insert(std::make_pair(animation_key, p_animation2D));
	}

	//Animation Speed
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_animation_speed);
}
