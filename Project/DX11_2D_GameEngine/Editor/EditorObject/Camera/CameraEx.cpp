#include "stdafx.h"
#include "CameraEx.h"

#include <DX11_2D_GameEngine_Lib/TimeManager.h>
#include <DX11_2D_GameEngine_Lib/InputManager.h>

#include <DX11_2D_GameEngine_Lib/Transform.h>

void CameraEx::FinalUpdate()
{
	//ī�޶� ����
	ControlEditorCamera();

	//�� ���(��Ʈ����) ������Ʈ
	UpdateViewMatrix();
	//���� ���(��Ʈ����) ������Ʈ
	UpdateProjectionMatrix();
}

void CameraEx::ControlEditorCamera()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	auto rotation = transform->GetRotation().ToEulerAngle(); //ī�޶� ȸ�� ��
	rotation.z = 0.0f;

	auto right = transform->GetRightVector(); //ī�޶��� ���� ���� ����
	auto up = transform->GetUpVector(); //ī�޶��� ���� ���� ����
	auto forward = transform->GetForwardVector(); //ī�޶��� ���� ���� ����

	Vector3 movement_speed = Vector3::Zero;

	//���콺 ������ ��ư�� ������ ���� ������
	if(MOUSE_BUTTON_PRESS(KeyCode::CLICK_RIGHT))
	{
	    //�������� ī�޶� �ٶ󺸴� �������� ����
		if (KEY_PRESS(KeyCode::KEY_W))
			movement_speed += forward * m_speed * DELTA_TIME_F;

		//�������� ī�޶� �ٶ󺸴� �������� ����
		else if (KEY_PRESS(KeyCode::KEY_S))
			movement_speed -= forward * m_speed * DELTA_TIME_F;

		//�������� ī�޶� �ٶ󺸴� ���⿡�� ���������� �̵�
		if (KEY_PRESS(KeyCode::KEY_D))
			movement_speed += right * m_speed * DELTA_TIME_F;

		//�������� ī�޶� �ٶ󺸴� ���⿡�� �������� �̵�
		else if (KEY_PRESS(KeyCode::KEY_A))
			movement_speed -= right * m_speed * DELTA_TIME_F;

		//�������� ī�޶� �ٶ󺸴� ���⿡�� �������� �̵�
		if (KEY_PRESS(KeyCode::KEY_E))
			movement_speed += up * m_speed * DELTA_TIME_F;

		//�������� ī�޶� �ٶ󺸴� ���⿡�� �Ʒ������� �̵�
		else if (KEY_PRESS(KeyCode::KEY_Q))
			movement_speed -= up * m_speed * DELTA_TIME_F;

		//ī�޶� ��ġ ����
		transform->Translate(movement_speed);
		movement_speed *= m_drag;

		//���콺 �� ���� ��
		auto delta = MOUSE_MOVE;

		//���콺 �ٷ� ī�޶� ȸ��
		//���콺 ���� �������̶� ���� �ݴ� ��ǥ�� ���� ��.
		rotation.x += delta.y * 0.1f;
		rotation.y += delta.x * 0.1f;

		transform->SetRotation(Quaternion::QuaternionFromEulerAngle(rotation));
	}
}