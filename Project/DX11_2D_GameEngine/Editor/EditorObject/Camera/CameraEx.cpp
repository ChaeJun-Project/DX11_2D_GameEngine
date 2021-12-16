#include "stdafx.h"
#include "CameraEx.h"

#include <DX11_2D_GameEngine_Lib/TimeManager.h>
#include <DX11_2D_GameEngine_Lib/InputManager.h>

#include <DX11_2D_GameEngine_Lib/Transform.h>

void CameraEx::FinalUpdate()
{
	//카메라 제어
	if (m_is_active)
		ControlEditorCamera();

	//뷰 행렬(메트릭스) 업데이트
	UpdateViewMatrix();
	//투영 행렬(메트릭스) 업데이트
	UpdateProjectionMatrix();
}

void CameraEx::ControlEditorCamera()
{
	//마우스 오른쪽 버튼을 눌렀을 때만 움직임
	if (MOUSE_BUTTON_PRESS(KeyCode::CLICK_RIGHT))
	{
		switch (m_projection_type)
		{
		case ProjectionType::Orthographic:
			Control2DViewMode();
			break;
		case ProjectionType::Perspective:
			Control3DViewMode();
			break;
		}
	}
}

void CameraEx::Control3DViewMode()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	auto rotation = transform->GetRotation().ToEulerAngle(); //카메라 회전 값
	rotation.z = 0.0f;

	auto right = transform->GetRightVector(); //카메라의 오른 방향 벡터
	auto up = transform->GetUpVector(); //카메라의 위쪽 방향 벡터
	auto forward = transform->GetForwardVector(); //카메라의 정면 방향 벡터

	Vector3 movement_speed = Vector3::Zero;

	//편집전용 카메라가 바라보는 방향으로 전진
	if (KEY_PRESS(KeyCode::KEY_W))
		movement_speed += forward * m_speed * DELTA_TIME_F;

	//편집전용 카메라가 바라보는 방향으로 후진
	else if (KEY_PRESS(KeyCode::KEY_S))
		movement_speed -= forward * m_speed * DELTA_TIME_F;

	//편집전용 카메라가 바라보는 방향에서 오른쪽으로 이동
	if (KEY_PRESS(KeyCode::KEY_D))
		movement_speed += right * m_speed * DELTA_TIME_F;

	//편집전용 카메라가 바라보는 방향에서 왼쪽으로 이동
	else if (KEY_PRESS(KeyCode::KEY_A))
		movement_speed -= right * m_speed * DELTA_TIME_F;

	//편집전용 카메라가 바라보는 방향에서 위쪽으로 이동
	if (KEY_PRESS(KeyCode::KEY_E))
		movement_speed += up * m_speed * DELTA_TIME_F;

	//편집전용 카메라가 바라보는 방향에서 아래쪽으로 이동
	else if (KEY_PRESS(KeyCode::KEY_Q))
		movement_speed -= up * m_speed * DELTA_TIME_F;

	//마우스 휠 동작 값
	auto delta = MOUSE_MOVE;

	//마우스 휠로 카메라 회전
	//마우스 휠의 움직임이라 서로 반대 좌표가 들어가야 함.
	rotation.x += delta.y * 0.1f;
	rotation.y += delta.x * 0.1f;

	transform->SetRotation(Quaternion::QuaternionFromEulerAngle(rotation));

	//카메라 위치 변경
	transform->Translate(movement_speed);
	movement_speed *= m_drag;
}

void CameraEx::Control2DViewMode()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	auto rotation = transform->GetRotation().ToEulerAngle(); //카메라 회전 값
	rotation.z = 0.0f;

	auto right = transform->GetRightVector(); //카메라의 오른 방향 벡터
	auto up = transform->GetUpVector(); //카메라의 위쪽 방향 벡터
	auto forward = transform->GetForwardVector(); //카메라의 정면 방향 벡터

	Vector3 movement_speed = Vector3::Zero;

	//편집전용 카메라가 바라보는 방향에서 위쪽으로 이동
	if (KEY_PRESS(KeyCode::KEY_W))
		movement_speed += up * m_speed * DELTA_TIME_F;

	//편집전용 카메라가 바라보는 방향에서 아래쪽으로 이동
	else if (KEY_PRESS(KeyCode::KEY_S))
		movement_speed -= up * m_speed * DELTA_TIME_F;

	//편집전용 카메라가 바라보는 방향에서 오른쪽으로 이동
	if (KEY_PRESS(KeyCode::KEY_D))
		movement_speed += right * m_speed * DELTA_TIME_F;

	//편집전용 카메라가 바라보는 방향에서 왼쪽으로 이동
	else if (KEY_PRESS(KeyCode::KEY_A))
		movement_speed -= right * m_speed * DELTA_TIME_F;

	//카메라 위치 변경
	transform->Translate(movement_speed);
	movement_speed *= m_drag;
}
