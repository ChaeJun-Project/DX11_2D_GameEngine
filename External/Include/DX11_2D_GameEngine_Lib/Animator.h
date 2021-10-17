#pragma once

#include "IComponent.h"

class Animation;

class Animator : public IComponent
{
public:
	Animator();
	explicit Animator(const Animator& origin);
	~Animator();

	void Update() override;
	void FinalUpdate() override;

public:
    void Play(const bool& is_play_reverse = false);

public:
	void CreateAnimation
	(
		const std::string& animation_name,
		const std::string& animation_directory_path,
		const float& animation_playtime,
		const bool& is_loop
	);
	void LoadAnimation(const std::string& animation_file_path);
	void GetAnimation(const std::string& animation_name);

public:
	const std::shared_ptr<Animation> GetCurrentAnimation() { SAFE_GET_POINTER(this->m_p_current_animation); }
	void SetCurrentAnimation(const std::string& animation_name);

	void SetAnimationEvent(const std::string& animation_name, std::function<void(void)> func);

private:
	std::shared_ptr<Animation> m_p_current_animation = nullptr;
	std::map<std::string, std::shared_ptr<Animation>> m_p_animation_map;
};

