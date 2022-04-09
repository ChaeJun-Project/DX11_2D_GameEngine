#pragma once

class Transform;
class Animator2D;
class AudioSource;

class GameObjectController
{
public:
	GameObjectController() = default;
	virtual ~GameObjectController();

protected:
	virtual void Update_Move() {};
	virtual void Update_State() {};
	virtual void Update_Animation() {};

	void SetCurrentAnimation(const std::string& animation_name, const bool& is_loop = false, const bool& is_reverse = false);
	void SetCurrentAudioClip(const std::string& audio_clip_name, const float& volume, const bool& is_loop = false);

public:
    const int GetHp() const { return m_hp; }
	
	const Vector3 GetPosition();
	void SetPosition(const Vector3 position);

	const bool GetCurrentAnimationIsFinished();

	void SetReadyEventFunc(std::function<void(void)> p_ready_event_func) { m_p_ready_event_func = p_ready_event_func; }
	void SetPlayerWin(const bool& is_win) { m_is_win = is_win; }
	void SetWinEventFunc(std::function<void(void)> p_win_event_func) { m_p_win_event_func = p_win_event_func; }
	void SetDeadEventFunc(std::function<void(void)> p_dead_event_func) { m_p_dead_event_func = p_dead_event_func; }
	void SetDeadEventCallWait(const float& wait) { m_dead_event_call_wait = wait; }

protected:
	void SaveToScene(FILE* p_file);
	void LoadFromScene(FILE* p_file);

protected:
	//<summary>
	//������� �ʴ� GameObject�� ������ �� ����
	//</summary>
	int m_hp = 0;
	float m_run_speed = 0.0f;
	float m_jump_speed = 0.0f;
	float m_dash_speed = 0.0f;

	Transform* m_p_transform = nullptr;
	Animator2D* m_p_animator2D = nullptr;
	AudioSource* m_p_audio_source = nullptr;

	SideState m_side_state = SideState::Right;

	bool m_is_ready = false;
	bool m_is_win = false;
	bool m_is_dead = false;

	std::function<void(void)> m_p_ready_event_func = nullptr;
	std::function<void(void)> m_p_win_event_func = nullptr;
    std::function<void(void)> m_p_dead_event_func = nullptr;
	float m_dead_event_call_wait = 0.0f;
};
