#pragma once
#include "IResource.h"

class Animation : public IResource
{
public:
	Animation(const std::string resource_name);
	explicit Animation(const Animation& origin);
	~Animation();

	void LoadFromFile(const std::string& animation_directory_path) override;
	void SaveFile(const std::string& animation_directory_path) override;

	void Update();

public:
	//애니메이션 재생
	void Play();
	//애니메이션 역재생
	void PlayReverse();
	//애니메이션 정지
	void Stop();

public:
	const std::shared_ptr<Texture>& GetCurrentTexture() const
	{
		auto current_texture = this->m_p_texture_vector[this->m_current_frame_id];
		if (current_texture == nullptr)
			return nullptr;

		return current_texture;
	}
	const std::shared_ptr<Mesh> GetMesh() const { SAFE_GET_POINTER(this->m_p_mesh); }

	void SetAnimationEvent(const UINT& clip_index, std::function<void(void)> event_func);

	//애니메이션 1회 수행에 걸리는 시간 설정
	const float& GetAnimationTime() const { return this->m_animation_time; }
	void SetAnimationTime(const float& animation_time) { this->m_animation_time = animation_time; }


	//애니메이션 프레임당 유지 시간
	const float& GetAnimationFrameDuration() const { return this->m_animation_frame_duration; }

	//애니메이션 재생 속도
	const float& GetAnimationSpeed() const { return this->m_animation_speed; }
	void SetAnimationSpeed(const float& animation_speed) { this->m_animation_speed = animation_speed; }

	//현재 애니메이션 프레임 Index
	const int& GetCurrentFrameID() const { return this->m_current_frame_id; }

	const bool& GetIsPlaying() const { return this->m_is_playing; }
	const bool& GetIsPlayReverse() const { return this->m_is_play_reverse; }
	const bool& GetIsEnd() const { return this->m_is_end; }
	const bool& GetIsLoop() const { return this->m_is_loop; }

	void SetIsLoop(const bool& is_loop) { this->m_is_loop = is_loop; }

private:
	std::vector<std::shared_ptr<Texture>> m_p_texture_vector;
	std::shared_ptr<Mesh> m_p_mesh = nullptr;

	std::map<int, std::function<void(void)>> m_animation_event_func_map;

	float m_animation_frame_duration = 0.0f;
	float m_accumulate_time = 0.0f;
	float m_animation_time = 0.0f;
	float m_animation_speed = 1.0f;
	int m_current_frame_id = 0;

	bool m_is_playing = false;
	bool m_is_play_reverse = false;
	bool m_is_end = false;
	bool m_is_loop = false;

	friend class Animator;
};

