#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"

Camera::Camera(GameObject* p_game_object)
	:IComponent(ComponentType::Camera, p_game_object)
{
}

Camera::~Camera()
{

}

void Camera::Update()
{
	auto transform = m_p_game_object->GetComponent<Transform>();

	//������ ��
	auto rotation = transform->GetRotation().ToEulerAngle(); //ī�޶� ȸ�� ��
	rotation.z = 0.0f;

	auto right = transform->GetRightVector(); //ī�޶��� ���� ���� ����
	auto up = transform->GetUpVector(); //ī�޶��� ���� ���� ����
	auto forward = transform->GetForwardVector(); //ī�޶��� ���� ���� ����

	Vector3 movement_speed;

	auto input = InputManager::GetInstance();
	auto timer = TimeManager::GetInstance();


	if (input->KeyPress(KeyCode::KEY_W))
		movement_speed += forward * timer->GetDeltaTime_float();

	else if (input->KeyPress(KeyCode::KEY_S))
		movement_speed -= forward * timer->GetDeltaTime_float();

	if (input->KeyPress(KeyCode::KEY_D))
		movement_speed += right * timer->GetDeltaTime_float();

	else if (input->KeyPress(KeyCode::KEY_A))
		movement_speed -= right * timer->GetDeltaTime_float();

	if (input->KeyPress(KeyCode::KEY_E))
		movement_speed += up * timer->GetDeltaTime_float();

	else if (input->KeyPress(KeyCode::KEY_Q))
		movement_speed -= up * timer->GetDeltaTime_float();

	//ī�޶� ��ġ ����
	transform->Translate(movement_speed);
}

void Camera::FinalUpdate()
{
	//�� ���(��Ʈ����) ������Ʈ
	UpdateViewMatrix();
	//���� ���(��Ʈ����) ������Ʈ
	UpdateProjectionMatrix();

	g_cbuffer_wvpmatrix.view = this->m_view_matrix;
	g_cbuffer_wvpmatrix.projection = this->m_projection_matrix;

}

void Camera::UpdateViewMatrix()
{
	auto transform = this->m_p_game_object->GetComponent<Transform>();
	auto position = transform->GetTranslation(); //���忡���� ī�޶� ��ġ��
	auto up_vector = transform->GetUpVector(); //ī�޶��� �� ����
	auto forward_vector = transform->GetForwardVector(); //ī�޶��� �ٶ󺸴� ���� ����(���� ����)

	//ī�޶��� ��ġ�� �ݴ� �������� ��� ��ü�� �������
	//ī�޶��� ��ġ�� �߽��� ���¿��� ������Ʈ�� �� �� ����
	//���� position�� -1�� ���� ������ �� ����� ����
	this->m_view_matrix = Matrix::LookAtLH((position * -1.0f), forward_vector, up_vector);
}

void Camera::UpdateProjectionMatrix()
{
	auto resolution_x = static_cast<float>(Settings::GetInstance()->GetWindowWidth());
	auto resolution_y = static_cast<float>(Settings::GetInstance()->GetWindowHeight());

	switch (this->m_projection_type)
	{
		//���� ���� ���
	case ProjectionType::Orthographic:
		this->m_projection_matrix = Matrix::OrthoLH(resolution_x, resolution_y, this->m_near_z, this->m_far_z);
		break;

		//���� ���� ���
	case ProjectionType::Perspective:
		this->m_projection_matrix = Matrix::PerspectiveFovLH(this->m_fov, (resolution_x / resolution_y), this->m_near_z, this->m_far_z);
		break;
	}
}
