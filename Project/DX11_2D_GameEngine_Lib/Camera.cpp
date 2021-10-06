#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"

Camera::Camera(IObject* p_game_object)
	:IComponent(ComponentType::Camera, p_game_object)
{
}

void Camera::Update()
{
	auto transform = m_p_game_object->GetComponent<Transform>();

	//������ ��


	//���� �÷����� ��

	//transform->Translate();


}

void Camera::FinalUpdate()
{
	//�� ���(��Ʈ����) ������Ʈ
	UpdateViewMatrix();
	//���� ���(��Ʈ����) ������Ʈ
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	auto transform = m_p_game_object->GetComponent<Transform>();
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
