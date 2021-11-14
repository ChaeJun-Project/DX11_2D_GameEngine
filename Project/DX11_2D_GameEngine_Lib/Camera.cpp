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

	//�� �����Ӹ��� RenderManager�� ī�޶� ���
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
		//�ش� ī�޶� ȭ�鿡 ������ �� �ִ� ���̾���
		//Culling Layer�� ��ϵ��� ���� Layer�� �׸�
		if (this->m_culling_layer & (1 << layer_iter.first))
			continue;

		else
			layer_iter.second->Render();
	}
}

void Camera::UpdateViewMatrix()
{
	auto transform = m_p_owner_game_object->GetComponent<Transform>();
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
	auto settings = Core::GetInstance()->GetSettings();
	auto resolution_x = static_cast<float>(settings->GetWindowWidth());
	auto resolution_y = static_cast<float>(settings->GetWindowHeight());

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

void Camera::CullingLayer(UINT layer_index)
{
	//�̹� ī�޶� ��ϵ� Culling Layer���
	if (this->m_culling_layer & (1 << layer_index))
		this->m_culling_layer &= ~(1 << layer_index); //��� ����

	//ī�޶� ��ϵ� Culling Layer�� �ƴ϶��
	else
		this->m_culling_layer |= (1 << layer_index); //���
}
