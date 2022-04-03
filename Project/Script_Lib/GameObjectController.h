#pragma once

class Animator2D;

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

public:
    const int GetHp() const { return m_hp; }
	const bool GetCurrentAnimationIsFinished();

	const Vector3 GetPosition();
	void SetPosition(const Vector3 position);

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

	Transform* m_p_transform = nullptr;
	Animator2D* m_p_animator2D = nullptr;
};
