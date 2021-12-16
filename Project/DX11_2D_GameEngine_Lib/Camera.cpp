#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"

#include "Core.h"
#include "Settings.h"

#include "Shader.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "RenderManager.h"
#include "SpriteRenderer.h"
#include "ParticleSystem.h"

Camera::Camera()
	:IComponent(ComponentType::Camera)
{
}

Camera::~Camera()
{
	m_forward_object_vector.clear();
	m_particle_object_vector.clear();
	m_post_effect_object_vector.clear();
}

Camera::Camera(const Camera& origin)
	: IComponent(origin.GetComponentType())
{
	m_camera_index = origin.m_camera_index;
	m_projection_type = origin.m_projection_type;

	m_fov = origin.m_fov;
	m_near_z = origin.m_near_z;
	m_far_z = origin.m_far_z;

	m_view_matrix = origin.m_view_matrix;
	m_projection_matrix = origin.m_projection_matrix;

	m_culling_layer = origin.m_culling_layer;
}

void Camera::FinalUpdate()
{
	//�� ���(��Ʈ����) ������Ʈ
	UpdateViewMatrix();
	//���� ���(��Ʈ����) ������Ʈ
	UpdateProjectionMatrix();

	//�� �����Ӹ��� RenderManager�� ī�޶� ���
	RenderManager::GetInstance()->RegisterCamera(this, m_camera_index);

	if (m_camera_index == 0)
	{
		auto transform = m_p_owner_game_object->GetComponent<Transform>();
		g_cbuffer_program.view_position = transform->GetTranslation();
	}
}

void Camera::UpdateMatrix()
{
	g_cbuffer_wvpmatrix.view = m_view_matrix;
	g_cbuffer_wvpmatrix.projection = m_projection_matrix;
}

void Camera::SortObjects()
{
	m_forward_object_vector.clear();
	m_particle_object_vector.clear();
	m_post_effect_object_vector.clear();

	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
	auto layer_map = current_scene->GetLayerMap();

	for (auto& layer_iter : layer_map)
	{
		//�ش� ī�޶� ȭ�鿡 ������ �� �ִ� ���̾���
		//Culling Layer�� ��ϵ��� ���� Layer�� �׸�
		if (m_culling_layer & (1 << layer_iter.first))
			continue;

		//ȭ�鿡 �������� ���
		else
		{
			const std::vector<GameObject*>& object_vector = layer_iter.second->GetGameObjects();

			RenderTimePointType render_time_point = RenderTimePointType::NONE;

			for (UINT i = 0; i < object_vector.size(); ++i)
			{
				//SpriteRenderer Component
				SpriteRenderer* sprtie_renderer = object_vector[i]->GetComponent<SpriteRenderer>();
				//ParticleSystem Component
				ParticleSystem* particle_system = object_vector[i]->GetComponent<ParticleSystem>();

				//�ش� ������Ʈ�� SpriteRenderer ������Ʈ�� �����ϰ� �ִٸ�
				if (sprtie_renderer != nullptr)
				{
					//Mesh, Material, Shader �� �ϳ��� ������ �Ǿ����� �ʴٸ�
					//�׸��� ����
					if (sprtie_renderer->GetMesh() == nullptr ||
						sprtie_renderer->GetMaterial() == nullptr ||
						sprtie_renderer->GetMaterial()->GetShader() == nullptr)

					{
						continue;
					}

					render_time_point = sprtie_renderer->GetMaterial()->GetShader()->GetRenderTimePointType();
				}

				//�ش� ������Ʈ�� ParticleSystem ������Ʈ�� �����ϰ� �ִٸ�
				else if (particle_system != nullptr)
				{
					//Mesh, Material, Shader �� �ϳ��� ������ �Ǿ����� �ʴٸ�
					//�׸��� ����
					if (particle_system->GetMesh() == nullptr ||
						particle_system->GetMaterial() == nullptr ||
						particle_system->GetMaterial()->GetShader() == nullptr)

					{
						continue;
					}

					render_time_point = particle_system->GetMaterial()->GetShader()->GetRenderTimePointType();
				}

				else
				{
					render_time_point = RenderTimePointType::Forward;
				}

				//�� ������Ʈ�� �׸��� ������ ���� �ش��ϴ� ���Ϳ� �߰�
				switch (render_time_point)
				{
				case RenderTimePointType::Forward:
					m_forward_object_vector.emplace_back(object_vector[i]);
					break;
				case RenderTimePointType::Particle:
					m_particle_object_vector.emplace_back(object_vector[i]);
					break;
				case RenderTimePointType::PostEffect:
					m_post_effect_object_vector.emplace_back(object_vector[i]);
					break;
				}
			}
		}
	}
}

void Camera::RenderForwardObjects()
{
	for (UINT i = 0; i < m_forward_object_vector.size(); ++i)
	{
		m_forward_object_vector[i]->Render();
	}
}

void Camera::RenderParticleObjects()
{
	for (UINT i = 0; i < m_particle_object_vector.size(); ++i)
	{
		m_particle_object_vector[i]->Render();
	}
}

void Camera::RenderPostEffectObjects()
{
	for (UINT i = 0; i < m_post_effect_object_vector.size(); ++i)
	{
		//Post Effect�� ����� �ؽ�ó�� �������� �����ϴ� �κ�
		RenderManager::GetInstance()->CopyPostEffect();
		m_post_effect_object_vector[i]->Render();
	}
}

void Camera::CullingLayer(UINT layer_index)
{
	//�̹� ī�޶� ��ϵ� Culling Layer���
	if (m_culling_layer & (1 << layer_index))
		m_culling_layer &= ~(1 << layer_index); //��� ����

	//ī�޶� ��ϵ� Culling Layer�� �ƴ϶��
	else
		m_culling_layer |= (1 << layer_index); //���
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
	m_view_matrix = Matrix::LookAtLH((position * -1.0f), forward_vector, up_vector);
}

void Camera::UpdateProjectionMatrix()
{
	auto resolution = RenderManager::GetInstance()->GetResolution();

	switch (m_projection_type)
	{
		//���� ���� ���
	case ProjectionType::Orthographic:
		m_projection_matrix = Matrix::OrthoLH(resolution.x * m_size.x, resolution.y * m_size.y, m_near_z, m_far_z);
		break;

		//���� ���� ���
	case ProjectionType::Perspective:
		m_projection_matrix = Matrix::PerspectiveFovLH(m_fov, (resolution.x / resolution.y), m_near_z, m_far_z);
		break;
	}
}

const Vector3 Camera::Picking()
{
	if (m_camera_index != 0)
		return Vector3::Zero;

	Vector2 mouse_position = InputManager::GetInstance()->GetMousePosition();

	Vector3 mouse_world_position = ScreenToWorld(mouse_position);

	return mouse_world_position;
}

const Vector3 Camera::ScreenToWorld(const Vector2& mouse_position)
{
	Vector2 screen_resolution = Vector2
	(
		static_cast<float>(Core::GetInstance()->GetSettings()->GetWindowWidth()),
		static_cast<float>(Core::GetInstance()->GetSettings()->GetWindowHeight())
	);

	Vector3 pick_ray_view_space = Vector3
	(
		((2.0f * mouse_position.x) / screen_resolution.x) - 1.0f,
		1.0f - ((2.0f * mouse_position.y) / screen_resolution.y),
		1.0f //���� ������ ��
	);

	Vector3 world_position;

	Matrix view_projection_inverse = (m_view_matrix * m_projection_matrix).Inverse();
	world_position = pick_ray_view_space * view_projection_inverse;

	return world_position;
}

void Camera::SetProjectionType(const ProjectionType& projection_type)
{
	m_projection_type = projection_type;

	if (m_projection_type == ProjectionType::Orthographic)
	{
		auto transform = m_p_owner_game_object->GetComponent<Transform>();
		transform->SetRotation(Quaternion::QuaternionFromEulerAngle(Vector3::Zero));
	}
}