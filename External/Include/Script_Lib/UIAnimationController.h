#pragma once

class Texture;
class ImageRenderer;

class AudioSource;

class UIAnimationController
{
public:
	UIAnimationController() = default;
	virtual ~UIAnimationController();

protected:
	void LoadTextures();
	void PlayUIAnimation();
	void StopUIAnimation();
    void UpdateUIAnimation();

	void SetCurrentAudioClip(const std::string& audio_clip_name, const float& volume, const bool& is_loop = false);

public:
    const bool GetIsPlaying() const { return m_is_playing; }
	void SetEndEventFunc(std::function<void(void)> p_end_event_func) { m_p_end_event_func = p_end_event_func; }
	
protected:
	void SaveToScene(FILE* p_file);
	void LoadFromScene(FILE* p_file);

protected:
	std::string m_textrue_path;
	std::vector<std::shared_ptr<Texture>> m_p_textrue_vector;
	std::shared_ptr<Texture> m_p_current_texture = nullptr;

	ImageRenderer* p_image_renderer = nullptr;
	AudioSource* m_p_audio_source = nullptr;

	bool m_is_playing = false;

	float m_duration = 0.0f;
	float m_accumulate_time = 0.0f;
	int m_current_frame_id = 0;

private:
	std::function<void(void)> m_p_end_event_func = nullptr;
};