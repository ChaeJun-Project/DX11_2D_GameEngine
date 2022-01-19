#pragma once
#include "IComponent.h"

class Texture;
class Animation2D;

class Animator2D final : public IComponent
{
public:
	Animator2D();
	explicit Animator2D(const Animator2D& origin);
	~Animator2D();

	virtual void Start() override;
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
	//Animation2D Create
	const std::shared_ptr<Animation2D>& CreateAnimation(const std::string& animation_name, const std::shared_ptr<Texture>& p_atlas_texture);
	
public:
    //Animator2D Index
	const int& GetAnimator2DID() const { return m_animator2D_index; }
	void SetAnimator2DID(const int& animator2D_index) { m_animator2D_index = animator2D_index; }

	std::map<std::string, std::shared_ptr<Animation2D>>& GetAnimationMap() { return m_p_animation_map; }
	const std::shared_ptr<Animation2D>& GetAnimation(const std::string& animation_name);
	const std::shared_ptr<Animation2D>& GetCurrentAnimation() { SAFE_GET_POINTER(m_p_current_animation); }
	void SetCurrentAnimation(const std::string& animation_name);

	const std::shared_ptr<Texture>& GetAtlasTexture();
	
	//Animation Playing
	const bool& GetIsPlaying() const { return m_is_playing; }

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

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Animator2D);

private:
    int m_animator2D_index = -1;

	std::shared_ptr<Animation2D> m_p_current_animation = nullptr;
	std::map<std::string, std::shared_ptr<Animation2D>> m_p_animation_map;

	bool m_is_playing = false;
	bool m_is_play_reverse = false;
	bool m_is_loop = false;

	float m_animation_speed = 1.0f;
};

