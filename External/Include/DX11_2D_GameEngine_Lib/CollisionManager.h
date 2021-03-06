#pragma once

union Collider_ID
{
	struct 
	{
		UINT left_id;
		UINT right_id;
	}; 
	ULONGLONG ID;
};

class Collider2D;

class CollisionManager final : public Singleton<CollisionManager>
{
	SINGLETON(CollisionManager);
	CollisionManager();
	~CollisionManager();

public:
    void Update();

private:
    void CollisionLayerUpdate(const UINT& left_layer, const UINT& right_layer);
	const bool IsCollision(Collider2D* p_left_collider2D, Collider2D* p_right_collider2D);

public:
	void CheckLayer(const UINT& left_layer, const UINT& right_layer);

public:
	const std::vector<UINT>& GetCollisionCheckVector() { return m_collision_check_vector; }

private:
    std::vector<UINT> m_collision_check_vector; //Layer 간의 충돌 체크 매트릭스

	std::unordered_map<ULONGLONG, bool> m_collision_check_unmap; //충돌체 간의 이전 프레임 충돌 정보 
};

