#include "stdafx.h"
#include "CollisionManager.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "GameObject.h"
#include "Collider2D.h"

CollisionManager::CollisionManager()
{
	m_check_vector.reserve(32);
	m_check_vector.resize(32);

	ResetCheckVector();
}

CollisionManager::~CollisionManager()
{
	m_check_vector.clear();
	m_check_vector.shrink_to_fit();

	m_collision_check_map.clear();
}

void CollisionManager::Update()
{
	//행 개수만큼 반복
	for (UINT row = 0; row < MAX_LAYER; ++row)
	{
		//중복되는 경우를 뺀 열의 개수만큼 반복
		for (UINT column = row; column < MAX_LAYER; ++column)
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

	const auto& left_layer_game_objects = current_scene->GetLayer(left_layer)->GetGameObjects();
	const auto& right_layer_game_objects = current_scene->GetLayer(right_layer)->GetGameObjects();

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

			iter = m_collision_check_map.find(colider_id.ID);

			//해당하는 Colider ID를 찾지 못했다면
			if (iter == m_collision_check_map.end())
			{
				m_collision_check_map.insert(std::make_pair(colider_id.ID, false));
				iter = m_collision_check_map.find(colider_id.ID);
			}

			//현재 충돌한 경우
			if (IsCollision(left_collider, right_collider))
			{
				//이전에도 충돌하고 있었던 경우
				if (iter->second)
				{
					//둘중 하나 또는 모두 삭제 예정인 경우
					if (left_layer_game_objects[i]->IsDead() || right_layer_game_objects[j]->IsDead())
					{
					    //충돌처리 해제
						left_collider->OnCollisionExit(right_collider);
						right_collider->OnCollisionExit(left_collider);
						iter->second = false;
					}

					//둘중 하나 또는 모두 삭제 예정이 아닌 경우
					//계속 충돌하고 있음
					else
					{
						left_collider->OnCollision(right_collider);
						right_collider->OnCollision(left_collider);
					}
				}

				//이전에는 충돌하지 않았던 경우(처음 충돌하는 경우)
				else
				{
					//둘 다 삭제 예정이 아닌 경우
					if (!left_layer_game_objects[i]->IsDead() && !right_layer_game_objects[j]->IsDead())
					{
					    //충돌처리
						left_collider->OnCollisionEnter(right_collider);
						right_collider->OnCollisionEnter(left_collider);
						iter->second = true;
					}
				}
			}

			//현재 충돌하지 않은 경우
			else
			{
				//과거에 충돌했었던 경우
				if (iter->second)
				{
					//충돌처리 해제
					left_collider->OnCollisionExit(right_collider);
					right_collider->OnCollisionExit(left_collider);
					iter->second = false;
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
	UINT row = left_layer; //행
	UINT column = right_layer; //열

	//더 작은 값의 Layer을 행으로, 더 큰 값의 Layer는 열로 사용
	if (row > column)
	{
		row = right_layer;
		column = left_layer;
	}
	
	//이미 체크가 된 경우
	if (m_check_vector[row] & (1 << column))
	{
		//해당 행렬의 비트값을 뺌
		m_check_vector[row] &= ~(1 << column);
	}

	else
		m_check_vector[row] |= (1 << column);
}