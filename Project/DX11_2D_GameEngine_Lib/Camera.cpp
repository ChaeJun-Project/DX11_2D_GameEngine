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
	//뷰 행렬(메트릭스) 업데이트
	UpdateViewMatrix();
	//투영 행렬(메트릭스) 업데이트
	UpdateProjectionMatrix();

	//매 프레임마다 RenderManager에 카메라 등록
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
		//해당 카메라가 화면에 보여줄 수 있는 레이어라면
		//Culling Layer에 등록되지 않은 Layer만 그림
		if (m_culling_layer & (1 << layer_iter.first))
			continue;

		//화면에 보여지는 경우
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

				//해당 오브젝트가 SpriteRenderer 컴포넌트를 포함하고 있다면
				if (sprtie_renderer != nullptr)
				{
					//Mesh, Material, Shader 중 하나라도 설정이 되어있지 않다면
					//그리지 않음
					if (sprtie_renderer->GetMesh() == nullptr ||
						sprtie_renderer->GetMaterial() == nullptr ||
						sprtie_renderer->GetMaterial()->GetShader() == nullptr)

					{
						continue;
					}

					render_time_point = sprtie_renderer->GetMaterial()->GetShader()->GetRenderTimePointType();
				}

				//해당 오브젝트가 ParticleSystem 컴포넌트를 포함하고 있다면
				else if (particle_system != nullptr)
				{
					//Mesh, Material, Shader 중 하나라도 설정이 되어있지 않다면
					//그리지 않음
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

				//각 오브젝트의 그리는 시점에 따라 해당하는 벡터에 추가
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
		//Post Effect가 적용된 텍스처를 누적으로 복사하는 부분
		RenderManager::GetInstance()->CopyPostEffect();
		m_post_effect_object_vector[i]->Render();
	}
}

void Camera::CullingLayer(UINT layer_index)
{
	//이미 카메라에 등록된 Culling Layer라면
	if (m_culling_layer & (1 << layer_index))
		m_culling_layer &= ~(1 << layer_index); //등록 해제

	//카메라에 등록된 Culling Layer가 아니라면
	else
		m_culling_layer |= (1 << layer_index); //등록
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
	m_view_matrix = Matrix::LookAtLH((position * -1.0f), forward_vector, up_vector);
}

void Camera::UpdateProjectionMatrix()
{
	auto resolution = RenderManager::GetInstance()->GetResolution();

	switch (m_projection_type)
	{
		//직교 투영 모드
	case ProjectionType::Orthographic:
		m_projection_matrix = Matrix::OrthoLH(resolution.x * m_size.x, resolution.y * m_size.y, m_near_z, m_far_z);
		break;

		//원근 투영 모드
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
		1.0f //투영 윈도우 값
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