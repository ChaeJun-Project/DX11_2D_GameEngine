#include "stdafx.h"
#include "CollisionManager.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "GameObject.h"
#include "Collider2D.h"

CollisionManager::CollisionManager()
{
	m_collision_check_vector.resize(MAX_LAYER, 0);
}

CollisionManager::~CollisionManager()
{
	m_collision_check_vector.clear();
	m_collision_check_vector.shrink_to_fit();

	m_collision_check_unmap.clear();
}

void CollisionManager::Update()
{
	//�� ������ŭ �ݺ�(MAX_LAYER: 32)
	for (UINT row = 0; row < MAX_LAYER; ++row)
	{
		//�ߺ��Ǵ� ��츦 �� ���� ������ŭ �ݺ�
		for (UINT column = row; column < MAX_LAYER; ++column)
		{
			//�̸� �浹 üũ ������ �� ��츸
			if (m_collision_check_vector[row] & (1 << column))
				CollisionLayerUpdate(row, column);
		}
	}
}

void CollisionManager::CollisionLayerUpdate(const UINT& left_layer, const UINT& right_layer)
{
	auto current_scene = SCENE_MANAGER->GetCurrentScene();

	const auto& left_layer_game_objects = current_scene->GetLayer(left_layer)->GetGameObjects();
	const auto& right_layer_game_objects = current_scene->GetLayer(right_layer)->GetGameObjects();

	std::unordered_map<ULONGLONG, bool>::iterator iter;

	for (UINT i = 0; i < static_cast<UINT>(left_layer_game_objects.size()); ++i)
	{
		if (left_layer_game_objects[i]->GetComponent<Collider2D>() == nullptr)
			continue;

		for (UINT j = 0; j < static_cast<UINT>(right_layer_game_objects.size()); ++j)
		{
			if (right_layer_game_objects[j]->GetComponent<Collider2D>() == nullptr
				|| left_layer_game_objects[i] == right_layer_game_objects[j])
				continue;

			auto left_collider = left_layer_game_objects[i]->GetComponent<Collider2D>();
			auto right_collider = right_layer_game_objects[j]->GetComponent<Collider2D>();

			//�� �浹ü ���� ���̵� ����
			Collider_ID colider_id;
			colider_id.left_id = left_collider->GetObjectID();
			colider_id.right_id = right_collider->GetObjectID();

			iter = m_collision_check_unmap.find(colider_id.ID);

			//�ش��ϴ� Colider ID�� ã�� ���ߴٸ�
			if (iter == m_collision_check_unmap.end())
			{
				m_collision_check_unmap.insert(std::make_pair(colider_id.ID, false));
				iter = m_collision_check_unmap.find(colider_id.ID);
			}

			//���� �浹�� ���
			if (IsCollision(left_collider, right_collider))
			{
				//�������� �浹�ϰ� �־��� ���
				if (iter->second)
				{
					//�� �� �ϳ��� GameObject�� ��Ȱ��ȭ ������ ���
					//�� �� �ϳ��� GameObject�� �������� ������ ���
					//�� �� �ϳ��� Collider2D Box�� ��Ȱ��ȭ ������ ��� 
					if (!left_layer_game_objects[i]->GetIsActive() || !right_layer_game_objects[j]->GetIsActive()
						|| left_layer_game_objects[i]->IsDead() || right_layer_game_objects[j]->IsDead()
						|| !left_collider->GetIsActive() || !right_collider->GetIsActive())
					{
						//�浹ó�� ����
						left_collider->OnCollisionExit(right_collider);
						right_collider->OnCollisionExit(left_collider);
						iter->second = false;

						if (left_layer_game_objects[i]->IsDead() || right_layer_game_objects[j]->IsDead())
							m_collision_check_unmap.erase(iter->first);
					}

					//�� �� �ϳ� �Ǵ� ��� ���� ������ �ƴ� ���
					//��� �浹�ϰ� ����
					else
					{
						left_collider->OnCollisionStay(right_collider);
						right_collider->OnCollisionStay(left_collider);
					}
				}

				//�������� �浹���� �ʾҴ� ���(ó�� �浹�ϴ� ���)
				else
				{
					//�� �� �ϳ��� GameObject�� ��Ȱ��ȭ ������ ���
					//�� �� �ϳ��� Collider2D Box�� ��Ȱ��ȭ ������ ��� 
					if (!left_layer_game_objects[i]->GetIsActive() || !right_layer_game_objects[j]->GetIsActive()
						|| !left_collider->GetIsActive() || !right_collider->GetIsActive())
						continue;

					//�� ���� GameObject�� ��� �������� ���°� �ƴ� ���
					if (!left_layer_game_objects[i]->IsDead() && !right_layer_game_objects[j]->IsDead())
					{
						//�浹ó��
						left_collider->OnCollisionEnter(right_collider);
						right_collider->OnCollisionEnter(left_collider);

						iter->second = true;
					}
				}
			}

			//���� �浹���� ���� ���
			else
			{
				//���ſ� �浹�߾��� ���
				if (iter->second)
				{
					//�浹ó�� ����
					left_collider->OnCollisionExit(right_collider);
					right_collider->OnCollisionExit(left_collider);
					iter->second = false;
				}

				//���ſ��� �浹���� �ʾҴ� ���
				else
				{
					m_collision_check_unmap.erase(iter->first);
				}
			}
		}
	}

}

const bool CollisionManager::IsCollision(Collider2D* p_left_collider2D, Collider2D* p_right_collider2D)
{
	static Vector3 local_pos_array[4]
	{
		Vector3(-0.5f, 0.5f, 0.0f),
		Vector3(0.5f, 0.5f, 0.0f),
		Vector3(0.5f, -0.5f, 0.0f),
		Vector3(-0.5f, -0.5f, 0.0f),
	};

	for (UINT i = 0; i < 4; ++i)
	{
		local_pos_array[i] * p_left_collider2D->GetDefaultSize();
	}

	const auto& left_collider_world = p_left_collider2D->GetColliderWorldMatrix();
	const auto& right_collider_world = p_right_collider2D->GetColliderWorldMatrix();

	Vector3 projection_axis_array[4];
	Vector3 center = Vector3::Zero * left_collider_world - Vector3::Zero * right_collider_world;

	projection_axis_array[0] = local_pos_array[1] * left_collider_world - local_pos_array[0] * left_collider_world;
	projection_axis_array[1] = local_pos_array[3] * left_collider_world - local_pos_array[0] * left_collider_world;

	projection_axis_array[2] = local_pos_array[1] * right_collider_world - local_pos_array[0] * right_collider_world;
	projection_axis_array[3] = local_pos_array[3] * right_collider_world - local_pos_array[0] * right_collider_world;

	for (UINT i = 0; i < 4; ++i)
	{
		projection_axis_array[i].z = 0.0f;
	}
	center.z = 0.0f;

	//�� ������ 4���� ���͸� ������Ų �Ÿ��� ������ ���� ������ ���� ���̸� ���ؼ� �и����� �����ϴ��� Ȯ��
	//�и����� 1���� ������ ��� �� ������Ʈ�� �浹���� �ʾ����� �ǹ�
	for (UINT i = 0; i < 4; ++i)
	{
		auto axis_vector = projection_axis_array[i];
		axis_vector.Normalize();

		auto half = 0.0f;
		for (UINT j = 0; j < 4; ++j)
		{
			half += Math::Abs(axis_vector.Dot(projection_axis_array[j])) * 0.5f; //�� ������ ���� ����� ������ ����� ���;� ��
		}

		auto center_dist = Math::Abs(axis_vector.Dot(center));

		if (half < center_dist)
		{
			return false;
		}
	}
	return true;
}

void CollisionManager::CheckLayer(const UINT& left_layer, const UINT& right_layer)
{
	UINT row = left_layer; //��
	UINT column = right_layer; //��

	//�� ���� ���� Layer�� ������, �� ū ���� Layer�� ���� ���
	if (row > column)
	{
		row = right_layer;
		column = left_layer;
	}

	//�̹� üũ�� �� ���
	if (m_collision_check_vector[row] & (1 << column))
	{
		//�ش� ����� ��Ʈ���� ��
		m_collision_check_vector[row] &= ~(1 << column);
	}

	else
		m_collision_check_vector[row] |= (1 << column);
}