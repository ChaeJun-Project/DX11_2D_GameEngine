#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"

#include "Core.h"
#include "Settings.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "RenderManager.h"

Camera::Camera()
	:IComponent(ComponentType::Camera)
{
}

Camera::Camera(const Camera& origin)
	: IComponent(origin.GetComponentType())
{
	this->m_camera_index = origin.m_camera_index;
	this->m_projection_type = origin.m_projection_type;

	this->m_fov = origin.m_fov;
	this->m_near_z = origin.m_near_z;
	this->m_far_z = origin.m_far_z;
	this->m_speed = origin.m_speed;

	this->m_view_matrix = origin.m_view_matrix;
	this->m_projection_matrix = origin.m_projection_matrix;

	this->m_culling_layer = origin.m_culling_layer;
}

void Camera::Update()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();

	//편집일 때
	auto rotation = transform->GetRotation().ToEulerAngle(); //카메라 회전 값
	rotation.z = 0.0f;

	auto right = transform->GetRightVector(); //카메라의 오른 방향 벡터
	auto up = transform->GetUpVector(); //카메라의 위쪽 방향 벡터
	auto forward = transform->GetForwardVector(); //카메라의 정면 방향 벡터

	Vector3 movement_speed;

	auto input = InputManager::GetInstance();
	auto timer = TimeManager::GetInstance();

	if (input->KeyPress(KeyCode::KEY_W))
		movement_speed += forward * m_speed * timer->GetDeltaTime_float();

	else if (input->KeyPress(KeyCode::KEY_S))
		movement_speed -= forward * m_speed * timer->GetDeltaTime_float();

	if (input->KeyPress(KeyCode::KEY_D))
		movement_speed += right * m_speed * timer->GetDeltaTime_float();

	else if (input->KeyPress(KeyCode::KEY_A))
		movement_speed -= right * m_speed * timer->GetDeltaTime_float();

	if (input->KeyPress(KeyCode::KEY_E))
		movement_speed += up * m_speed * timer->GetDeltaTime_float();

	else if (input->KeyPress(KeyCode::KEY_Q))
		movement_speed -= up * m_speed * timer->GetDeltaTime_float();

	//카메라 위치 변경
	transform->Translate(movement_speed);
}

void Camera::FinalUpdate()
{
	//뷰 행렬(메트릭스) 업데이트
	UpdateViewMatrix();
	//투영 행렬(메트릭스) 업데이트
	UpdateProjectionMatrix();

	g_cbuffer_wvpmatrix.view = this->m_view_matrix;
	g_cbuffer_wvpmatrix.projection = this->m_projection_matrix;

	//매 프레임마다 RenderManager에 카메라 등록
	RenderManager::GetInstance()->RegisterCamera(this, this->m_camera_index);

	if (m_camera_index == 0)
	{
	    auto transform = m_p_owner_game_object->GetComponent<Transform>();
		g_cbuffer_program.view_position = transform->GetTranslation();
	}
}

void Camera::Render()
{
	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
	auto layer_map = current_scene->GetLayerMap();

	for (auto& layer_iter : layer_map)
	{
		//해당 카메라가 화면에 보여줄 수 있는 레이어라면
		//Culling Layer에 등록되지 않은 Layer만 그림
		if (this->m_culling_layer & (1 << layer_iter.first))
			continue;

		else
			layer_iter.second->Render();
	}
}

void Camera::UpdateViewMatrix()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();
	auto position = transform->GetTranslation(); //월드에서의 카메라 위치값
	auto up_vector = transform->GetUpVector(); //카메라의 업 벡터
	auto forward_vector = transform->GetForwardVector(); //카메라의 바라보는 방향 벡터(전면 벡터)

	//카메라의 위치의 반대 방향으로 모든 물체를 움직어야
	//카메라의 위치가 중심인 상태에서 오브젝트를 볼 수 있음
	//따라서 position에 -1을 곱한 값으로 뷰 행렬을 만듦
	this->m_view_matrix = Matrix::LookAtLH((position * -1.0f), forward_vector, up_vector);
}

void Camera::UpdateProjectionMatrix()
{
	auto settings = Core::GetInstance()->GetSettings();
	auto resolution_x = static_cast<float>(settings->GetWindowWidth());
	auto resolution_y = static_cast<float>(settings->GetWindowHeight());

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

void Camera::CullingLayer(UINT layer_index)
{
	//이미 카메라에 등록된 Culling Layer라면
	if (this->m_culling_layer & (1 << layer_index))
		this->m_culling_layer &= ~(1 << layer_index); //등록 해제

	//카메라에 등록된 Culling Layer가 아니라면
	else
		this->m_culling_layer |= (1 << layer_index); //등록
}
