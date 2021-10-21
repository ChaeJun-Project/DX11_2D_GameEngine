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
	//행 개수만큼 반복
	for (UINT row = 0; row < static_cast<UINT>(this->m_check_vector.size()); ++row)
	{
		//중복되는 경우를 뺀 열의 개수만큼 반복
		for (UINT column = row; column < static_cast<UINT>(this->m_check_vector.size()); ++column)
		{
			//이미 체크가 된 경우
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

			//두 충돌체 조합 아이디 생성
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
					// 이전에도 충돌 하고 있었다.
					if (left_layer_game_objects[i]->IsDead() || right_layer_game_objects[j]->IsDead())
					{
						// 근데 둘중 하나가 삭제 예정이라면, 충돌 해제시켜준다.
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
					// 이전에는 충돌하지 않았다.
					// 근데 둘중 하나가 삭제 예정이라면, 충돌하지 않은것으로 취급
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
				// 현재 충돌하고 있지 않다.
				if (iter->second)
				{
					// 이전에는 충돌하고 있었다.
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

	//각 축으로 4개의 벡터를 투영시킨 거리와 중점을 이은 벡터의 투영 길이를 비교해서 분리축이 존재하는지 확인
	//분리축이 1개라도 존재할 경우 두 오브젝트는 충돌하지 않았음을 의미
	for (UINT i = 0; i < 4; ++i)
	{
		auto axis_vector = projection_axis_array[i];
		axis_vector.Normalize();

		auto half = 0.0f;
		for (UINT j = 0; j < 4; ++j)
		{
			half += Math::Abs(axis_vector.Dot(projection_axis_array[j])) * 0.5f; //두 벡터의 내적 결과는 무조건 양수가 나와야 함
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
	UINT row; //행
	UINT column; //열

	//더 작은 값의 Layer을 행으로, 더 큰 값의 Layer는 열로 사용
	left_layer < right_layer ? row = left_layer, column = right_layer : row = right_layer, column = left_layer;

	//이미 체크가 된 경우
	if (m_check_vector[row] & (1 << column))
	{
		//해당 행렬의 비트값을 뺌
		m_check_vector[row] &= ~(1 << column);
	}

	else
		m_check_vector[row] |= (1 << column);
}