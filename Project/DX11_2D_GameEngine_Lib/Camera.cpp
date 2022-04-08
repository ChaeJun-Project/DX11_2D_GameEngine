#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"

#include "Core.h"
#include "Settings.h"

#include "Shader.h"
#include "Material.h"

#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

#include "SpriteRenderer.h"
#include "Light2D.h"
#include "ParticleRenderer.h"

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
	: IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;

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
	RENDER_MANAGER->RegisterCamera(this, m_camera_index);

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

	auto current_scene = SCENE_MANAGER->GetCurrentScene();
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

			RenderTimePointType render_time_point = RenderTimePointType::None;

			for (UINT i = 0; i < object_vector.size(); ++i)
			{
				//SpriteRenderer Component
				auto p_sprtie_renderer = object_vector[i]->GetComponent<SpriteRenderer>();
				//ParticleRenderer Component
				auto p_particle_renderer = object_vector[i]->GetComponent<ParticleRenderer>();

				//해당 오브젝트가 SpriteRenderer 컴포넌트를 포함하고 있다면
				if (p_sprtie_renderer != nullptr)
				{
					//Mesh, Material, Shader 중 하나라도 설정이 되어있지 않다면
					//그리지 않음
					if (p_sprtie_renderer->GetMesh() == nullptr ||
						p_sprtie_renderer->GetMaterial() == nullptr ||
						p_sprtie_renderer->GetMaterial()->GetShader() == nullptr)

					{
						continue;
					}

					render_time_point = p_sprtie_renderer->GetMaterial()->GetShader()->GetRenderTimePointType();
				}

				//해당 오브젝트가 ParticleRenderer 컴포넌트를 포함하고 있다면
				else if (p_particle_renderer != nullptr)
				{
					//Mesh, Material, Shader 중 하나라도 설정이 되어있지 않다면
					//그리지 않음
					if (p_particle_renderer->GetMesh() == nullptr ||
						p_particle_renderer->GetMaterial() == nullptr ||
						p_particle_renderer->GetMaterial()->GetShader() == nullptr)

					{
						continue;
					}

					render_time_point = p_particle_renderer->GetMaterial()->GetShader()->GetRenderTimePointType();
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
		//최상위 부모 GameObject인 경우
		if (!m_forward_object_vector[i]->HasParent())
		{
			//활성화가 되어있다면 렌더링
			if (m_forward_object_vector[i]->GetIsActive())
				m_forward_object_vector[i]->Render();
		}

		//부모 GameObject를 가지고 있는 경우
		else
		{
			GameObject* p_parent_game_object = m_forward_object_vector[i];
			auto is_render = true;
			//현재 GameObject의 부모 GameObject를 시작으로 탐색하여
			//부모 GameObject가 하나라도 비활성화 상태라면
			//현재 GameObject를 그리지 않음
			while (p_parent_game_object != nullptr)
			{
				p_parent_game_object = p_parent_game_object->GetParent();

				if (p_parent_game_object != nullptr && !p_parent_game_object->GetIsActive())
				{ 
					is_render = false;
					break;
				}
			}

			if (!is_render)
				continue;

			if (m_forward_object_vector[i]->GetIsActive())
				m_forward_object_vector[i]->Render();
		}
	}
}

void Camera::RenderParticleObjects()
{
	for (UINT i = 0; i < m_particle_object_vector.size(); ++i)
	{
		//최상위 부모 GameObject인 경우
		if (!m_particle_object_vector[i]->HasParent())
		{
			//활성화가 되어있다면 렌더링
			if (m_particle_object_vector[i]->GetIsActive())
				m_particle_object_vector[i]->Render();
		}

		//부모 GameObject를 가지고 있는 경우
		else
		{
			GameObject* p_parent_game_object = m_particle_object_vector[i];
			auto is_render = true;
			//현재 GameObject의 부모 GameObject를 시작으로 탐색하여
			//부모 GameObject가 하나라도 비활성화 상태라면
			//현재 GameObject를 그리지 않음
			while (p_parent_game_object != nullptr)
			{
				p_parent_game_object = p_parent_game_object->GetParent();

				if (p_parent_game_object != nullptr && !p_parent_game_object->GetIsActive())
				{
					is_render = false;
					break;
				}
			}

			if (!is_render)
				continue;

			if (m_particle_object_vector[i]->GetIsActive())
				m_particle_object_vector[i]->Render();
		}
	}
}

void Camera::RenderPostEffectObjects()
{
	for (UINT i = 0; i < m_post_effect_object_vector.size(); ++i)
	{
		//Post Effect가 적용된 텍스처를 누적으로 복사하는 부분
		RENDER_MANAGER->CopyPostEffect();
		//최상위 부모 GameObject인 경우
		if (!m_post_effect_object_vector[i]->HasParent())
		{
			//활성화가 되어있다면 렌더링
			if (m_post_effect_object_vector[i]->GetIsActive())
				m_post_effect_object_vector[i]->Render();
		}

		//부모 GameObject를 가지고 있는 경우
		else
		{
			GameObject* p_parent_game_object = m_post_effect_object_vector[i];
			auto is_render = true;
			//현재 GameObject의 부모 GameObject를 시작으로 탐색하여
			//부모 GameObject가 하나라도 비활성화 상태라면
			//현재 GameObject를 그리지 않음
			while (p_parent_game_object != nullptr)
			{
				p_parent_game_object = p_parent_game_object->GetParent();

				if (p_parent_game_object != nullptr && !p_parent_game_object->GetIsActive())
				{
					is_render = false;
					break;
				}
			}

			if (!is_render)
				continue;

			if (m_post_effect_object_vector[i]->GetIsActive())
				m_post_effect_object_vector[i]->Render();
		}
	}
}

void Camera::CullingLayer(const UINT& layer_index)
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
	Vector2 resolution = Vector2::Zero;

	switch (SCENE_MANAGER->GetClientState())
	{
	case 1: //Game
		resolution = SETTINGS->GetGameResolution();
		break;
	case 2: //Editor
		resolution = RENDER_MANAGER->GetClientResolution();
		break;
	}

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
	Vector2 mouse_position = INPUT_MANAGER->GetMousePosition();

	if (!RENDER_MANAGER->CheckClickedEditorSceneRect(mouse_position))
		return Vector3::Zero;

	Vector3 mouse_world_position = ScreenToWorld(mouse_position);

	return mouse_world_position;
}

const Vector3 Camera::ScreenToWorld(const Vector2& mouse_position)
{
	auto screen_resolution = RENDER_MANAGER->GetClientResolution();
	auto screen_offset = RENDER_MANAGER->GetScreenOffset();
	auto mouse_relative_position = mouse_position - screen_offset;

	Vector3 pick_ray_view_space = Vector3
	(
		((2.0f * mouse_relative_position.x) / screen_resolution.x) - 1.0f,
		1.0f - ((2.0f * mouse_relative_position.y) / screen_resolution.y),
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

void Camera::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Camera Index
	fprintf(p_file, "[Index]\n");
	fprintf(p_file, "%d\n", m_camera_index);

	//Camera Culling Layer
	fprintf(p_file, "[Culling Layer]\n");
	fprintf(p_file, "%d\n", m_culling_layer);

	//카메라 투영 타입
	fprintf(p_file, "[Projection]\n");
	auto projection_type = static_cast<UINT>(m_projection_type);
	fprintf(p_file, "%d\n", projection_type);

	//ProjectionType::Orthographic(직교 투영 전용)
	//Size가 늘어남에 따라 한 화면에 보여지는 화면의 해상도 증가(커질수록 줌아웃, 작아질수록 줌인)
	if (m_projection_type == ProjectionType::Orthographic)
	{
		fprintf(p_file, "[Size]\n");
		FILE_MANAGER->FPrintf_Vector2(m_size, p_file);
	}

	//Field of View
	fprintf(p_file, "[Fov]\n");
	fprintf(p_file, "%f\n", m_fov);

	fprintf(p_file, "[Near]\n");
	fprintf(p_file, "%f\n", m_near_z);

	fprintf(p_file, "[Far]\n");
	fprintf(p_file, "%f\n", m_far_z);
}

void Camera::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Camera Index
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &m_camera_index);

	//Camera Culling Layer
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &m_culling_layer);

	//카메라 투영 타입
	FILE_MANAGER->FScanf(char_buffer, p_file);
	int projection_type = -1;
	fscanf_s(p_file, "%d\n", &projection_type);
	m_projection_type = static_cast<ProjectionType>(projection_type);

	//ProjectionType::Orthographic(직교 투영 전용)
	//Size가 늘어남에 따라 한 화면에 보여지는 화면의 해상도 증가(커질수록 줌아웃, 작아질수록 줌인)
	if (m_projection_type == ProjectionType::Orthographic)
	{
		FILE_MANAGER->FScanf(char_buffer, p_file);
		FILE_MANAGER->FScanf_Vector2(m_size, p_file);
	}

	//Field of View
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_fov);

	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_near_z);

	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_far_z);
}
