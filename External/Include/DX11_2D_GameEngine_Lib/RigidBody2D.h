#pragma once

#include "IComponent.h"

class RigidBody2D final : public IComponent
{
public:
	RigidBody2D();
	explicit RigidBody2D(const RigidBody2D& origin);
	~RigidBody2D();

	void Update() override;
	void FinalUpdate() {};

private:
	void Move();

public:
	void SetGround(const bool& is_ground);
	bool GetIsGround() const { return m_is_ground; }

	void SetWall(const bool& is_right, const bool& is_wall);
	bool GetIsRightWall() const { return m_is_right_wall; }
	bool GetIsLeftWall() const { return m_is_left_wall; }

	void SetCeiling(const bool& is_ceiling);

public:
	void AddForce(const Vector2& force) { m_force += force; }

	float GetSpeed() const { return m_velocity.Length(); }
	Vector2 GetVelocity() const { return m_velocity; }
	void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }
	void AddVelocity(const Vector2& velocity) { m_velocity += velocity; }

	float GetGravity() const { return m_gravity; }
	void SetGravity(const float& gravity) { m_gravity = gravity; }

	float GetMass() { return m_mass; }
	void SetMass(const float& mass)
	{
		if (mass <= 0.0f)
			m_mass = 0.01f;
		else
			m_mass = mass;
	}

	void SetFrictionCoeff(const float& friction_coeff) { m_friction_coeff = friction_coeff; }

	void SetAccelAlpha(const Vector2& accel_alpha) { m_accel_alpha = accel_alpha; }

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(RigidBody2D);

private:
	Vector2	m_force = Vector2::Zero;	    // 크기, 방향
	Vector2	m_accel = Vector2::Zero;		// 가속도
	Vector2	m_accel_alpha = Vector2::Zero;	// 추가 가속도 

	Vector2	m_velocity = Vector2::Zero;	    // 속도(크기 : 속력, 방향)

	bool m_is_ground = false;
	bool m_is_right_wall = false;
	bool m_is_left_wall = false;
	bool m_is_ceiling = false;
	float m_gravity = 0.0f;					// 중력
	float m_mass = 0.01f;		            // 질량

	float m_friction_coeff = 0.0f;	        // 마찰 계수
};

