#pragma once

class Animator2D;
class Texture;

class SpriteAnimation final : public IResource
{
public:
	SpriteAnimation(const std::string& animation2D_resource_name);
	explicit SpriteAnimation(const SpriteAnimation& origin);
	~SpriteAnimation();

	void Update();
	void FinalUpdate();

public:
	void CreateAnimationFrame(const SpriteAnimation_Frame& animation2D_frame)
	{
		m_animation_frame_vector.emplace_back(animation2D_frame);
	}

	const int DeleteAnimationFrame(const UINT& frame_index)
	{
		m_animation_frame_vector.erase(m_animation_frame_vector.begin() + frame_index);

		return frame_index > 0 ? frame_index - 1 : 0;
	}

public:
	//애니메이션 재생
	void Play();
	//애니메이션 정지
	void Pause();
	//애니메이션 중지
	void Stop();

public:
    //Animation Atlas Texture
	void SetAtlasTexture(const std::shared_ptr<Texture>& p_atlas_texture) { m_p_atlas_texture = p_atlas_texture; }
	const std::shared_ptr<Texture>& GetAtlasTexture() { SAFE_GET_POINTER(m_p_atlas_texture); }

	//Animation Event
	void SetAnimationEvent(const UINT& clip_index, std::function<void(void)> event_func);
	void DoAnimationEvent(const UINT& clip_index);
	void ResetAnimationEventFlag();

	//현재 애니메이션 프레임 Index
	const int& GetCurrentFrameID() const { return m_current_frame_id; }

	//현재 애니메이션 프레임에 대한 정보
	const int& GetAnimationFrameCount() const { return static_cast<int>(m_animation_frame_vector.size()); }
	const bool& HasAnimationFrame() const { return !m_animation_frame_vector.empty(); }
	const SpriteAnimation_Frame& GetAnimationFrame(const UINT& frame_index)
	{
		if (!m_animation_frame_vector.empty())
			return m_animation_frame_vector[frame_index];
	}
	void SetAnimationFrame(const UINT& frame_index, const SpriteAnimation_Frame& animation_frame)
	{
		if (m_animation_frame_vector.size() - 1 >= frame_index)
		{
			m_animation_frame_vector[frame_index] = animation_frame;
		}
	}
	const SpriteAnimation_Data& GetCurrentFrameData() const { return m_animation_frame_vector[m_current_frame_id].animation2D_data; }

public:
	bool SaveToFile(const std::string& animation2D_path) override;
	bool LoadFromFile(const std::string& animation2D_path) override;

public:
	CLONE(SpriteAnimation);

private:
	Animator2D* m_p_owner_animator2D = nullptr;

	std::vector<SpriteAnimation_Frame> m_animation_frame_vector;
	std::shared_ptr<Texture> m_p_atlas_texture;

	std::map<UINT, std::pair<bool, std::function<void(void)>>> m_animation_event_func_map;

	float m_accumulate_time = 0.0f;
	float m_animation_time = 0.0f;
	int m_current_frame_id = 0;

	bool m_is_finished = false;
	bool m_is_paused = false;

	friend class Animator2D;
};

