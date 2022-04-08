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
	//�� ���(��Ʈ����) ������Ʈ
	UpdateViewMatrix();
	//���� ���(��Ʈ����) ������Ʈ
	UpdateProjectionMatrix();

	//�� �����Ӹ��� RenderManager�� ī�޶� ���
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
		//�ش� ī�޶� ȭ�鿡 ������ �� �ִ� ���̾���
		//Culling Layer�� ��ϵ��� ���� Layer�� �׸�
		if (m_culling_layer & (1 << layer_iter.first))
			continue;

		//ȭ�鿡 �������� ���
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

				//�ش� ������Ʈ�� SpriteRenderer ������Ʈ�� �����ϰ� �ִٸ�
				if (p_sprtie_renderer != nullptr)
				{
					//Mesh, Material, Shader �� �ϳ��� ������ �Ǿ����� �ʴٸ�
					//�׸��� ����
					if (p_sprtie_renderer->GetMesh() == nullptr ||
						p_sprtie_renderer->GetMaterial() == nullptr ||
						p_sprtie_renderer->GetMaterial()->GetShader() == nullptr)

					{
						continue;
					}

					render_time_point = p_sprtie_renderer->GetMaterial()->GetShader()->GetRenderTimePointType();
				}

				//�ش� ������Ʈ�� ParticleRenderer ������Ʈ�� �����ϰ� �ִٸ�
				else if (p_particle_renderer != nullptr)
				{
					//Mesh, Material, Shader �� �ϳ��� ������ �Ǿ����� �ʴٸ�
					//�׸��� ����
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
		//�ֻ��� �θ� GameObject�� ���
		if (!m_forward_object_vector[i]->HasParent())
		{
			//Ȱ��ȭ�� �Ǿ��ִٸ� ������
			if (m_forward_object_vector[i]->GetIsActive())
				m_forward_object_vector[i]->Render();
		}

		//�θ� GameObject�� ������ �ִ� ���
		else
		{
			GameObject* p_parent_game_object = m_forward_object_vector[i];
			auto is_render = true;
			//���� GameObject�� �θ� GameObject�� �������� Ž���Ͽ�
			//�θ� GameObject�� �ϳ��� ��Ȱ��ȭ ���¶��
			//���� GameObject�� �׸��� ����
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
		//�ֻ��� �θ� GameObject�� ���
		if (!m_particle_object_vector[i]->HasParent())
		{
			//Ȱ��ȭ�� �Ǿ��ִٸ� ������
			if (m_particle_object_vector[i]->GetIsActive())
				m_particle_object_vector[i]->Render();
		}

		//�θ� GameObject�� ������ �ִ� ���
		else
		{
			GameObject* p_parent_game_object = m_particle_object_vector[i];
			auto is_render = true;
			//���� GameObject�� �θ� GameObject�� �������� Ž���Ͽ�
			//�θ� GameObject�� �ϳ��� ��Ȱ��ȭ ���¶��
			//���� GameObject�� �׸��� ����
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
		//Post Effect�� ����� �ؽ�ó�� �������� �����ϴ� �κ�
		RENDER_MANAGER->CopyPostEffect();
		//�ֻ��� �θ� GameObject�� ���
		if (!m_post_effect_object_vector[i]->HasParent())
		{
			//Ȱ��ȭ�� �Ǿ��ִٸ� ������
			if (m_post_effect_object_vector[i]->GetIsActive())
				m_post_effect_object_vector[i]->Render();
		}

		//�θ� GameObject�� ������ �ִ� ���
		else
		{
			GameObject* p_parent_game_object = m_post_effect_object_vector[i];
			auto is_render = true;
			//���� GameObject�� �θ� GameObject�� �������� Ž���Ͽ�
			//�θ� GameObject�� �ϳ��� ��Ȱ��ȭ ���¶��
			//���� GameObject�� �׸��� ����
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

void Camera::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Camera Index
	fprintf(p_file, "[Index]\n");
	fprintf(p_file, "%d\n", m_camera_index);

	//Camera Culling Layer
	fprintf(p_file, "[Culling Layer]\n");
	fprintf(p_file, "%d\n", m_culling_layer);

	//ī�޶� ���� Ÿ��
	fprintf(p_file, "[Projection]\n");
	auto projection_type = static_cast<UINT>(m_projection_type);
	fprintf(p_file, "%d\n", projection_type);

	//ProjectionType::Orthographic(���� ���� ����)
	//Size�� �þ�� ���� �� ȭ�鿡 �������� ȭ���� �ػ� ����(Ŀ������ �ܾƿ�, �۾������� ����)
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

	//ī�޶� ���� Ÿ��
	FILE_MANAGER->FScanf(char_buffer, p_file);
	int projection_type = -1;
	fscanf_s(p_file, "%d\n", &projection_type);
	m_projection_type = static_cast<ProjectionType>(projection_type);

	//ProjectionType::Orthographic(���� ���� ����)
	//Size�� �þ�� ���� �� ȭ�鿡 �������� ȭ���� �ػ� ����(Ŀ������ �ܾƿ�, �۾������� ����)
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
