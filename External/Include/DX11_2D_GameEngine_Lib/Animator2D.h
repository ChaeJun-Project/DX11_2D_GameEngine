#pragma once
#include "IComponent.h"

class Texture;
class SpriteAnimation;

class Animator2D final : public IComponent
{
public:
	Animator2D();
	explicit Animator2D(const Animator2D& origin);
	~Animator2D();

	virtual void Update() override;
	virtual void FinalUpdate() override;

public:
	//애니메이션 재생
	void Play();
	//애니메이션 정지
	void Pause();
	//애니메이션 중지
	void Stop();

public:
    //Current Animation Atlas Texture
	const std::shared_ptr<Texture>& GetCurrentAnimationAtlasTexture();

	//Sprite Animation
	void AddAnimation(const std::string& sprite_animation_name);
	void DeleteAnimation(const std::string& sprite_animation_name);
	std::map<std::string, std::shared_ptr<SpriteAnimation>>& GetAnimationMap() { return m_p_sprite_animation_map; }
	const bool HasAnimation(const std::string& sprite_animation_name);
	const std::shared_ptr<SpriteAnimation>& GetCurrentAnimation() { SAFE_GET_POINTER(m_p_current_animation); }
	void SetCurrentAnimation(const std::string& animation_name);

	const bool GetCurrentAnimationIsFinished();

	//Animation Playing
	const bool& GetIsPlaying() const { return m_is_playing; }
	void SetIsPlaying(const bool& is_playing) { m_is_playing = is_playing; }

	//Animation Play Reverse
	bool& GetIsPlayReverse() { return m_is_play_reverse; }
	void SetIsPlayReverse(const bool& is_play_reverse) { m_is_play_reverse = is_play_reverse; }

	//Animation Play Loop
	bool& GetIsLoop() { return m_is_loop; }
	void SetIsLoop(const bool& is_loop) { m_is_loop = is_loop; }

	//Animation Speed
	const float& GetAnimationSpeed() const { return m_animation_speed; }
	void SetAnimationSpeed(const float& animation_speed) { m_animation_speed = animation_speed; }

	//Animation Event
	void SetAnimationEvent(const std::string& animation_name, const UINT& clip_index, std::function<void(void)> event_func);

private:
    void SetMeshScale(); //SetCurrentAnimation에서 애니메이션이 변경될 때 호출

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(Animator2D);

private:
	std::map<std::string, std::shared_ptr<SpriteAnimation>> m_p_sprite_animation_map;
	std::shared_ptr<SpriteAnimation> m_p_current_animation = nullptr;

	bool m_is_playing = false;
	bool m_is_play_reverse = false;
	bool m_is_loop = false;

	float m_animation_speed = 1.0f;
};

