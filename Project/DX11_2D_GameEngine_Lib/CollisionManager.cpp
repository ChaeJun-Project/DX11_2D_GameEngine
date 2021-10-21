#include "stdafx.h"
#include "CollisionManager.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "GameObject.h"
#include "Collider2D.h"

CollisionManager::~CollisionManager()
{
	this->m_check_vector.clear();
	this->m_check_vector.shrink_to_fit();

	this->m_collision_check_map.clear();
}

void CollisionManager::Update()
{
	//�� ������ŭ �ݺ�
	for (UINT row = 0; row < static_cast<UINT>(this->m_check_vector.size()); ++row)
	{
		//�ߺ��Ǵ� ��츦 �� ���� ������ŭ �ݺ�
		for (UINT column = row; column < static_cast<UINT>(this->m_check_vector.size()); ++column)
		{
			//�̹� üũ�� �� ���
			if (m_check_vector[row] & (1 << column))
				CollisionLayerUpdate(row, column);
		}
	}
}

void CollisionManager::CollisionLayerUpdate(const UINT& left_layer, const UINT& right_layer)
{
	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();

	const std::vector<GameObject*>& left_layer_game_objects = current_scene->GetLayer(left_layer)->GetGameObjects();
	const std::vector<GameObject*>& right_layer_game_objects = current_scene->GetLayer(right_layer)->GetGameObjects();

	std::map<ULONGLONG, bool>::iterator iter;

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

			iter = this->m_collision_check_map.find(colider_id.ID);

			if (iter == this->m_collision_check_map.end())
			{
				this->m_collision_check_map.insert(std::make_pair(colider_id.ID, false));
				iter = this->m_collision_check_map.find(colider_id.ID);
			}

			if (IsCollision(left_collider, right_collider))
			{
				if (iter->second)
				{
					// �������� �浹 �ϰ� �־���.
					if (left_layer_game_objects[i]->IsDead() || right_layer_game_objects[j]->IsDead())
					{
						// �ٵ� ���� �ϳ��� ���� �����̶��, �浹 ���������ش�.
						left_collider->OnCollisionExit(right_collider);
						right_collider->OnCollisionExit(left_collider);
						iter->second = false;
					}
					else
					{
						left_collider->OnCollision(right_collider);
						right_collider->OnCollision(left_collider);
					}
				}

				else
				{
					// �������� �浹���� �ʾҴ�.
					// �ٵ� ���� �ϳ��� ���� �����̶��, �浹���� ���������� ���
					if (!left_layer_game_objects[i]->IsDead() && !right_layer_game_objects[j]->IsDead())
					{
						left_collider->OnCollisionEnter(right_collider);
						right_collider->OnCollisionEnter(left_collider);
						iter->second = true;
					}
				}
			}
			else
			{
				// ���� �浹�ϰ� ���� �ʴ�.
				if (iter->second)
				{
					// �������� �浹�ϰ� �־���.
					left_collider->OnCollisionExit(right_collider);
					right_collider->OnCollisionExit(left_collider);
					iter->second = false;
				}
			}
		}
	}

}

const bool CollisionManager::IsCollision(const std::shared_ptr<Collider2D>& p_left_collider2D, const std::shared_ptr<Collider2D>& p_right_collider2D)
{
	static Vector3 local_pos_array[4]
	{
		Vector3(-50.0f, 50.0f, 0.0f),
		Vector3(50.0f,50.0f, 0.0f),
		Vector3(50.0f, -50.0f, 0.0f),
		Vector3(-50.0f, -50.0f, 0.0f),
	};

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

		auto center_dist = Math::Abs(center.Dot(center));

		if (half < center_dist)
		{
			return false;
		}
	}
	return true;
}

void CollisionManager::CheckLayer(const UINT& left_layer, const UINT& right_layer)
{
	UINT row; //��
	UINT column; //��

	//�� ���� ���� Layer�� ������, �� ū ���� Layer�� ���� ���
	left_layer < right_layer ? row = left_layer, column = right_layer : row = right_layer, column = left_layer;

	//�̹� üũ�� �� ���
	if (m_check_vector[row] & (1 << column))
	{
		//�ش� ����� ��Ʈ���� ��
		m_check_vector[row] &= ~(1 << column);
	}

	else
		m_check_vector[row] |= (1 << column);
}