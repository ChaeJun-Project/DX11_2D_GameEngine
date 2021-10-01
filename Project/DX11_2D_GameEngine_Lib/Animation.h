#pragma once
#include "IResource.h"

class Animation : public IResource
{
public:
	Animation(const std::string resource_name);
	~Animation();

	const bool& LoadFromFile(const std::string& animation_path) override;

	void Update();
	void Render();

public:
	//애니메이션 재생
	void Play();
	//애니메이션 정지
	void Stop();
	//애니메이션 역재생
	void PlayReverse();

public:
	const std::vector<Texture> GetTextureVector() const { return this->m_texture_vector; }
	const Mesh<VertexColorTexture>* GetMesh() const { SAFE_GET_POINTER(this->m_p_mesh.Get()); }

	void SetAnimationEvent(const UINT& clip_index, std::function<void(void)> event_func);
	
	const bool& GetAnimationTime() const { return this->m_animation_time; }
	void SetAnimationTime(const float& animation_time) { this->m_animation_time = m_animation_time; }
	const bool& GetAnimationFrame() const { return this->m_animation_frame; }
	void SetAnimationFrame(const float& animation_frame) { this->m_animation_frame = animation_frame; }
	const bool& GetAnimationSpeed() const { return this->m_animation_speed; }
	void SetAnimationSpeed(const float& animation_speed) { this->m_animation_speed = animation_speed; }
	const UINT& GetCurrentClipID() const { return this->m_current_clip_id; }

	const bool& GetIsPlaying() const { return this->m_is_playing; }
	const bool& GetIsEnd() const { return this->m_is_end; }
	const bool& GetIsLoop() const { return this->m_is_loop; }
	void SetIsLoop(const bool* is_loop) { this->m_is_loop = m_is_loop; }

private:
	std::vector<Texture> m_texture_vector;
	ComPtr<Mesh<VertexColorTexture>> m_p_mesh = nullptr;

	std::map<UINT, std::function<void(void)>> m_animation_event_func_map;

	double m_animation_time = 0.0;
	float m_animation_frame = 0.0f;
	float m_animation_speed = 1.0f;
	UINT m_current_clip_id = 0;

	bool m_is_playing = false;
	bool m_is_end = false;
	bool m_is_loop = false;
};

