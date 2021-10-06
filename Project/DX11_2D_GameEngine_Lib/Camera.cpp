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

	//편집일 때


	//게임 플레이일 때

	//transform->Translate();


}

void Camera::FinalUpdate()
{
	//뷰 행렬(메트릭스) 업데이트
	UpdateViewMatrix();
	//투영 행렬(메트릭스) 업데이트
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	auto transform = m_p_game_object->GetComponent<Transform>();
	auto position = transform->GetTranslation(); //월드에서의 카메라 위치값
	auto up_vector = transform->GetUpVector(); //카메라의 업 벡터
	auto forward_vector = transform->GetForwardVector(); //카메라의 바라보는 방향 벡터(전면 벡터)

	//카메라가의 위치의 반대 방향으로 모든 물체를 움직어야
	//카메라의 위치가 중심인 상태에서 오브젝트를 볼 수 있음
	//따라서 position에 -1을 곱한 값으로 뷰 행렬을 만듦
	this->m_view_matrix = Matrix::LookAtLH((position * -1.0f), forward_vector, up_vector);
}

void Camera::UpdateProjectionMatrix()
{
	auto resolution_x = static_cast<float>(Settings::GetInstance()->GetWindowWidth());
	auto resolution_y = static_cast<float>(Settings::GetInstance()->GetWindowHeight());

	switch (this->m_projection_type)
	{
		//직교 투영 모드
	case ProjectionType::Orthographic:
		this->m_projection_matrix = Matrix::OrthoLH(resolution_x, resolution_y, this->m_near_z, this->m_far_z);
		break;

		//원근 투영 모드
	case ProjectionType::Perspective:
		this->m_projection_matrix = Matrix::PerspectiveFovLH(this->m_fov, (resolution_x / resolution_y), this->m_near_z, this->m_far_z);
		break;
	}
}
