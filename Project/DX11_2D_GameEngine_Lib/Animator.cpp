#include "stdafx.h"
#include "Animator.h"
#include "Shader.h"

Animator::Animator(IObject* p_game_object)
	:IComponent(ComponentType::Animator, p_game_object)
{
	m_p_shader = std::make_shared<Shader>("Standard_Shader");
	m_p_shader->AddAndCreateShader<VertexShader>("Shader/TextureShader.fx","VS", "vs_5_0");
	m_p_shader->AddAndCreateShader<PixelShader>("Shader/TextureShader.fx","PS", "ps_5_0");
}

void Animator::Update()
{
	if (this->m_p_current_animation == nullptr)
		return;

	this->m_p_current_animation->Update();
}

void Animator::FinalUpdate()
{
	Update();
}

void Animator::Render()
{
	if (this->m_p_current_animation == nullptr)
		return;
	this->m_p_shader->BindPipeline();
	this->m_p_current_animation->Render();
}

void Animator::AddAnimation(const std::string& animation_name, std::shared_ptr<Animation>& p_animation, const bool& is_default_animation)
{
	auto map_pair = this->m_animation_map.insert(std::make_pair(animation_name, p_animation));
	assert(map_pair.second);
	if (!map_pair.second)
		return;

	if (is_default_animation)
		this->m_p_default_animation = map_pair.first->second;
}

void Animator::SetCurrentAnimation(const std::string& animation_name)
{
	if (this->m_p_current_animation != nullptr)
		this->m_p_current_animation->Stop();

	auto animation_iter = this->m_animation_map.find(animation_name);
	//해당 애니메이션을 찾지 못했다면
	if (animation_iter != this->m_animation_map.end())
		return;

	this->m_p_current_animation = animation_iter->second;
}

void Animator::SetAnimationEvent(const std::string& animation_name, std::function<void(void)>)
{
	//TODO
}
