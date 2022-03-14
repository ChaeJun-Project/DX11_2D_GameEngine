#include "stdafx.h"
#include "RigidBody2D.h"

#include "GameObject.h"
#include "Transform.h"

RigidBody2D::RigidBody2D()
	:IComponent(ComponentType::RigidBody2D)
{
}

RigidBody2D::RigidBody2D(const RigidBody2D& origin)
	: IComponent(origin.m_component_type)
{
     m_gravity = origin.m_gravity;
	 m_mass = origin.m_mass;
}

RigidBody2D::~RigidBody2D()
{
}

void RigidBody2D::Update()
{
	//힘의 크기
	float force = m_force.Length();

	//힘이 0이 아니라면
	if (force != 0.0f)
	{
		//힘의 방향
		m_force.Normalize();

		//가속도의 크기 (a = F/m)
		float accel = force / m_mass;

		// 가속도
		m_accel = m_force * accel;
	}

	if (!m_is_right_wall && !m_is_left_wall)
		m_velocity += m_accel;

	if (!m_is_ground)
		m_velocity.y -= m_mass * m_gravity * DELTA_TIME_F;

	Move();

	m_force = Vector2::Zero;
	m_accel = Vector2::Zero;
}

void RigidBody2D::Move()
{
	//속도가 0이 아니면
	if (m_velocity.Length() != 0.0f)
	{
		auto transform = m_p_owner_game_object->GetComponent<Transform>();
		auto position = transform->GetLocalTranslation();

		position.x += (m_velocity.x / m_mass)* DELTA_TIME_F;
		if (!m_is_ceiling)
			position.y += m_velocity.y * DELTA_TIME_F;

		//캐릭터 위치 변경
		transform->SetTranslation(position);

		m_velocity.x = 0.0f;
	}
}

void RigidBody2D::SetGround(const bool& is_ground)
{
	m_is_ground = is_ground;
	if (m_is_ground)
	{
		m_velocity.y = 0.0f;
	}
}

void RigidBody2D::SetWall(const bool& is_right, const bool& is_wall)
{
	if (is_right)
		m_is_right_wall = is_wall;
	else
		m_is_left_wall = is_wall;
}

void RigidBody2D::SetCeiling(const bool& is_ceiling)
{
	m_is_ceiling = is_ceiling;
}

void RigidBody2D::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Gravity
	fprintf_s(p_file, "[Gravity]\n");
	fprintf_s(p_file, "%f\n", m_gravity);

	//Mass
	fprintf_s(p_file, "[Mass]\n");
	fprintf_s(p_file, "%f\n", m_mass);
}

void RigidBody2D::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Gravity
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_gravity);
	
	//Mass
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_mass);
}
