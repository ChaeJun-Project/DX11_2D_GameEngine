#include "stdafx.h"
#include "Animator2D.h"

#include "ResourceManager.h"
#include "SpriteAnimation.h"

#include "GameObject.h"
#include "Transform.h"

Animator2D::Animator2D()
	:IComponent(ComponentType::Animator2D)
{
}

Animator2D::Animator2D(const Animator2D& origin)
	: IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;

	//동일한 애니메이션의 정보를 바탕으로 복사 애니메이션 생성 및 추가
	for (const auto& animation_iter : origin.m_p_sprite_animation_map)
	{
		auto p_origin_sprite_animation = animation_iter.second;

		auto p_clone_sprite_animation_raw = p_origin_sprite_animation->Clone();
		auto p_clone_sprite_animation = std::shared_ptr<SpriteAnimation>(p_clone_sprite_animation_raw);
		p_clone_sprite_animation->m_p_owner_animator2D = this;

		auto copy_animation_iter = m_p_sprite_animation_map.insert(std::make_pair(p_clone_sprite_animation->GetResourceName(), p_clone_sprite_animation));
		auto result = copy_animation_iter.second;
		assert(result);
	}

	//Current Animation
	if (origin.m_p_current_animation != nullptr)
		m_p_current_animation = m_p_sprite_animation_map[origin.m_p_current_animation->GetResourceName()];

	//Animation Speed
	m_animation_speed = origin.m_animation_speed;
}

Animator2D::~Animator2D()
{
	for (auto& animation_iter : m_p_sprite_animation_map)
	{
		animation_iter.second.reset();
	}
	m_p_sprite_animation_map.clear();

	//Current Animation
	m_p_current_animation.reset();
}

void Animator2D::Update()
{
	if (m_p_current_animation == nullptr)
		return;

	if (m_is_playing)
		m_p_current_animation->Update();
}

void Animator2D::FinalUpdate()
{
	if (m_p_current_animation == nullptr)
		return;

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
	auto p_sprtie_animation = RESOURCE_MANAGER->GetResource<SpriteAnimation>(sprite_animation_name);

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
		if (m_p_current_animation->GetResourceName()._Equal(animation_name))
			return;

		else
			m_p_current_animation->Stop();
	}

	auto animation_iter = m_p_sprite_animation_map.find(animation_name);
	//해당 애니메이션을 찾지 못했다면
	if (animation_iter == m_p_sprite_animation_map.end())
		return;

	m_p_current_animation = animation_iter->second;
	SetMeshScale();

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

void Animator2D::SetMeshScale()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	auto frame_size = m_p_current_animation->GetCurrentFrame().frame_size;
	transform->SetMeshScale(static_cast<UINT>(frame_size.x), static_cast<UINT>(frame_size.y));

	transform->UpdateConstantBuffer();
}

void Animator2D::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

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
		RESOURCE_MANAGER->SaveResource<SpriteAnimation>(animation2D.second, p_file);
	}

	//Current Animation
	fprintf(p_file, "[Current Animation]\n");
	if (m_p_current_animation != nullptr)
		fprintf(p_file, "%s\n", m_p_current_animation->GetResourceName().c_str());
	else
		fprintf(p_file, "%s\n", "None");

	//Animation Speed
	fprintf(p_file, "[Animation Speed]\n");
	fprintf(p_file, "%f\n", m_animation_speed);
}

void Animator2D::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

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
		auto animation_key = std::string(char_buffer);

		//Load Sprite Animation Resource
		std::shared_ptr<SpriteAnimation> p_animation2D = nullptr;
		RESOURCE_MANAGER->LoadResource<SpriteAnimation>(p_animation2D, p_file);
		p_animation2D->m_p_owner_animator2D = this;
		m_p_sprite_animation_map.insert(std::make_pair(animation_key, p_animation2D));
	}

	//Current Animation
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);
	auto current_animation_key = std::string(char_buffer);
	if (!current_animation_key._Equal("None"))
		m_p_current_animation = m_p_sprite_animation_map[current_animation_key];

	//Animation Speed
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_animation_speed);
}
