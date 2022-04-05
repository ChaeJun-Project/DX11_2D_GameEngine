#include "stdafx.h"
#include "ProjectileController.h"

#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>

ProjectileController::~ProjectileController()
{
	m_p_transform = nullptr;
	m_p_animator2D = nullptr;
}

void ProjectileController::SetCurrentAnimation(const std::string& animation_name, const bool& is_loop, const bool& is_reverse)
{
	if (m_p_animator2D == nullptr)
		return;

	m_p_animator2D->SetCurrentAnimation(animation_name);
	m_p_animator2D->SetIsLoop(is_loop);
	m_p_animator2D->SetIsPlayReverse(is_reverse);
}

void ProjectileController::UpdateSide()
{
	//바라보는 방향에 따라 회전
	//왼쪽
	if (m_side_state == SideState::Left)
		m_p_transform->SetRotation(Quaternion::Identity);

	//오른쪽
	else
		m_p_transform->SetRotation(Quaternion::QuaternionFromEulerAngle(Vector3(0.0f, 180.0f, 0.0f)));
}

void ProjectileController::UpdateMove()
{
	if (m_p_transform == nullptr || m_fire_direction == Vector3::Zero)
		return;

	auto position = m_p_transform->GetLocalTranslation();
	auto move_speed = Vector3::Zero;

	move_speed = m_fire_speed * m_fire_direction * DELTA_TIME_F;

	m_p_transform->SetLocalTranslation(position + move_speed);
}

void ProjectileController::SaveToScene(FILE* p_file)
{
	//Fire Speed
	fprintf_s(p_file, "[Fire Speed]\n");
	fprintf_s(p_file, "%f\n", m_fire_speed);
}

void ProjectileController::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = { 0 };

	//Fire Speed
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_fire_speed);
}
