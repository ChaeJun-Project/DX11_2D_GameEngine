#include "stdafx.h"
#include "CameraEx.h"

#include <DX11_2D_GameEngine_Lib/TimeManager.h>
#include <DX11_2D_GameEngine_Lib/InputManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>

void CameraEx::FinalUpdate()
{
	//ī�޶� ����
	if (m_is_active)
		ControlEditorCamera();

	//�� ���(��Ʈ����) ������Ʈ
	UpdateViewMatrix();
	//���� ���(��Ʈ����) ������Ʈ
	UpdateProjectionMatrix();
}

void CameraEx::ControlEditorCamera()
{
	//���콺 ������ ��ư�� ������ ���� ������
	if (MOUSE_BUTTON_PRESS(Button::CLICK_RIGHT))
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

	auto rotation = transform->GetRotation().ToEulerAngle(); //ī�޶� ȸ�� ��
	rotation.z = 0.0f;

	auto position = transform->GetTranslation();

	auto right = transform->GetRightVector(); //ī�޶��� ���� ���� ����
	auto up = transform->GetUpVector(); //ī�޶��� ���� ���� ����
	auto forward = transform->GetForwardVector(); //ī�޶��� ���� ���� ����

	//���콺 ���� ��
	auto delta = MOUSE_MOVE;

	Vector3 movement_speed = Vector3::Zero;

	//�������� ī�޶� �ٶ󺸴� �������� ����
	if (KEY_PRESS(Key::KEY_W))
		movement_speed += forward * m_speed * DELTA_TIME_F;

	//�������� ī�޶� �ٶ󺸴� �������� ����
	else if (KEY_PRESS(Key::KEY_S))
		movement_speed -= forward * m_speed * DELTA_TIME_F;

	//�������� ī�޶� �ٶ󺸴� ���⿡�� ���������� �̵�
	if (KEY_PRESS(Key::KEY_D))
		movement_speed += right * m_speed * DELTA_TIME_F;

	//�������� ī�޶� �ٶ󺸴� ���⿡�� �������� �̵�
	else if (KEY_PRESS(Key::KEY_A))
		movement_speed -= right * m_speed * DELTA_TIME_F;

	//�������� ī�޶� �ٶ󺸴� ���⿡�� �������� �̵�
	if (KEY_PRESS(Key::KEY_E))
		movement_speed += up * m_speed * DELTA_TIME_F;

	//�������� ī�޶� �ٶ󺸴� ���⿡�� �Ʒ������� �̵�
	else if (KEY_PRESS(Key::KEY_Q))
		movement_speed -= up * m_speed * DELTA_TIME_F;

	//���콺�� ī�޶� ȸ��
	//���콺�� �������̶� ���� �ݴ� ��ǥ�� ���� ��.
	rotation.x += delta.y * 0.1f;
	rotation.y += delta.x * 0.1f;

	transform->SetRotation(Quaternion::QuaternionFromEulerAngle(rotation));

	//���콺 �ٷ� ī�޶� ���� �Ǵ� �ܾƿ�
	if (delta.z > 0) //����
		movement_speed += forward * m_zoom_speed * DELTA_TIME_F;

	else if (delta.z < 0) //�ܾƿ�
		movement_speed -= forward * m_zoom_speed * DELTA_TIME_F;

	//ī�޶� ��ġ ����
	transform->SetTranslation(position + movement_speed);
	movement_speed *= m_drag;
}

void CameraEx::Control2DViewMode()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	auto rotation = transform->GetRotation().ToEulerAngle(); //ī�޶� ȸ�� ��
	rotation.z = 0.0f;

	auto position = transform->GetTranslation();

	auto right = transform->GetRightVector(); //ī�޶��� ���� ���� ����
	auto up = transform->GetUpVector(); //ī�޶��� ���� ���� ����
	auto forward = transform->GetForwardVector(); //ī�޶��� ���� ���� ����

	Vector3 movement_speed = Vector3::Zero;

	//�������� ī�޶� �ٶ󺸴� ���⿡�� �������� �̵�
	if (KEY_PRESS(Key::KEY_W))
		movement_speed += up * m_speed * DELTA_TIME_F;

	//�������� ī�޶� �ٶ󺸴� ���⿡�� �Ʒ������� �̵�
	else if (KEY_PRESS(Key::KEY_S))
		movement_speed -= up * m_speed * DELTA_TIME_F;

	//�������� ī�޶� �ٶ󺸴� ���⿡�� ���������� �̵�
	if (KEY_PRESS(Key::KEY_D))
		movement_speed += right * m_speed * DELTA_TIME_F;

	//�������� ī�޶� �ٶ󺸴� ���⿡�� �������� �̵�
	else if (KEY_PRESS(Key::KEY_A))
		movement_speed -= right * m_speed * DELTA_TIME_F;

	//���콺 ���� ��
	auto delta = MOUSE_MOVE;
	//���콺 �ٷ� ī�޶� ���� �Ǵ� �ܾƿ�
	if (delta.z > 0) //����
	{
		m_size -= 0.1f;

		if (m_size.x <= 0.0f || m_size.y <= 0.0f)
			m_size = Vector2(0.1f, 0.1f);
	}

	else if (delta.z < 0) //�ܾƿ�
		m_size += 0.1f;

	//ī�޶� ��ġ ����
	transform->SetTranslation(position + movement_speed);
	movement_speed *= m_drag;
}
