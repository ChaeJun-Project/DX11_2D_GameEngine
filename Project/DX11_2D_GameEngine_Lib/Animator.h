#pragma once

#include "IComponent.h"

class Shader;
class Animation;

class Animator : public IComponent
{
public:
	Animator(IObject* p_game_object);
	~Animator() = default;

	void Update() override;
	void FinalUpdate() override;

	void Render();

public:
	void AddAnimation
	(
		const std::string& animation_name,
		std::shared_ptr<Animation>& p_animation,
		const bool& is_default_animation = false
	);

	const std::shared_ptr<Animation> GetCurrentAnimation() { SAFE_GET_POINTER(this->m_p_current_animation); }
	void SetCurrentAnimation(const std::string& animation_name);

	void SetAnimationEvent(const std::string& animation_name, std::function<void(void)>);

private:
    std::shared_ptr<Shader> m_p_shader = nullptr;
	std::shared_ptr<Animation> m_p_default_animation = nullptr;
	std::shared_ptr<Animation> m_p_current_animation = nullptr;
	std::map<std::string, std::shared_ptr<Animation>> m_animation_map;
};

