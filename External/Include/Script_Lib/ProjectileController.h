#pragma once

class Transform;
class Animator2D;

class ProjectileController
{
public:
	ProjectileController() = default;
	virtual ~ProjectileController();

protected:
	void SetCurrentAnimation(const std::string& animation_name, const bool& is_loop = false, const bool& is_reverse = false);
    void UpdateSide();
	void UpdateMove();

public:
    void SetFireDirection(const Vector3& fire_direction) { m_fire_direction = fire_direction; }
	void SetSideState(const SideState& side_state) { m_side_state = side_state; }

protected:
	void SaveToScene(FILE* p_file);
	void LoadFromScene(FILE* p_file);

protected:
	float m_fire_speed = 0.0f;

	Vector3 m_fire_direction = Vector3::Zero;

	SideState m_side_state = SideState::Left;

	Transform* m_p_transform = nullptr;
	Animator2D* m_p_animator2D = nullptr;
};