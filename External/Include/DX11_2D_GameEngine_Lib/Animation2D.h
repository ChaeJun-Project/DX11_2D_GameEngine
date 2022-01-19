#pragma once

class Animator2D;
class Texture;

class Animation2D final : public IResource
{
public:
	Animation2D(const std::string& animation2D_resource_name);
	explicit Animation2D(const Animation2D& origin);
	~Animation2D();

	void Update();
	void FinalUpdate();

public:
	void CreateAnimationFrame(const Animation2D_Frame& animation2D_frame)
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
	//Animation Name
	const std::string& GetAnimationName() { return m_resource_name; }

	//Animation Event
	void SetAnimationEvent(const UINT& clip_index, std::function<void(void)> event_func);
	void DoAnimationEvent(const UINT& clip_index);
	void ResetAnimationEventFlag();

	//현재 애니메이션 프레임 Index
	const int& GetCurrentFrameID() const { return m_current_frame_id; }

	//현재 애니메이션 프레임에 대한 정보
	const int& GetAnimationFrameCount() const { return static_cast<int>(m_animation_frame_vector.size()); }
	const bool& HasAnimationFrame() const { return !m_animation_frame_vector.empty(); }
	const Animation2D_Frame& GetAnimationFrame(const UINT& frame_index)
	{
		if (!m_animation_frame_vector.empty())
			return m_animation_frame_vector[frame_index];
	}
	void SetAnimationFrame(const UINT& frame_index, const Animation2D_Frame& animation_frame)
	{
		if (m_animation_frame_vector.size() - 1 >= frame_index)
		{
			m_animation_frame_vector[frame_index] = animation_frame;
		}
	}
	const Animation2D_Data& GetCurrentFrameData() const { return m_animation_frame_vector[m_current_frame_id].animation2D_data; }

public:
	bool SaveToFile(const std::string& animation2D_path) override;
	bool LoadFromFile(const std::string& animation2D_path) override;

public:
	CLONE(Animation2D);

private:
	Animator2D* m_p_owner_animator2D = nullptr;
	std::vector<Animation2D_Frame> m_animation_frame_vector;
	std::shared_ptr<Texture> m_p_atlas_texture;

	std::map<UINT, std::pair<bool, std::function<void(void)>>> m_animation_event_func_map;

	float m_accumulate_time = 0.0f;
	float m_animation_time = 0.0f;
	int m_current_frame_id = 0;

	bool m_is_finished = false;
	bool m_is_paused = false;

	friend class Animator2D;
};

