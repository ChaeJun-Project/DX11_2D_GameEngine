#pragma once

class SpriteAnimation;

enum AnimationState : UINT
{
	Stop = 0U,			//0000, 편집_재생중지(편집가능) 모드
	Play = 1U << 0,		//0001, 편집_재생 모드
	Pause = 1U << 1,	//0010, 편집_정지 모드
};

class GUI_SpritePlayer final
{
public:
	GUI_SpritePlayer();
	~GUI_SpritePlayer();

public:
    void Initialize();
	void Render();

private:
    void ShowSpriteAnimationPreview();
	void ShowButtons();

private:
	//SpriteAnimation
	std::shared_ptr<SpriteAnimation> m_p_current_animation2D = nullptr;
	int m_frame_index = 0;
	float m_accumulate_time = 0.0f;

	//Animation State
	UINT curret_animation_state = static_cast<UINT>(AnimationState::Stop);

	std::string gui_str = "SpritePlayer_";

	bool m_is_active = false;

	friend class GUI_SpriteEditor;
};

