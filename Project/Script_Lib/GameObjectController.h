#pragma once

class Animator2D;

class GameObjectController
{
public:
	GameObjectController() = default;
	virtual ~GameObjectController();

protected:
	virtual void Update_Move() = 0;
	virtual void Update_State() = 0;
	virtual void Update_Animation() = 0;

	void SetCurrentAnimation(std::string animation_name, const bool& is_loop = false, const bool& is_reverse = false);

public:
    int GetHp() const { return m_hp; }

protected:
	void SaveToScene(FILE* p_file);
	void LoadFromScene(FILE* p_file);

protected:
	//<summary>
	//사용하지 않는 GameObject가 존재할 수 있음
	//</summary>
	int m_hp = 0;
	float m_run_speed = 0.0f;
	float m_jump_speed = 0.0f;
	float m_dash_speed = 0.0f;

	SideState m_side_state = SideState::Right;

	Animator2D* m_p_animator2D = nullptr;
};
