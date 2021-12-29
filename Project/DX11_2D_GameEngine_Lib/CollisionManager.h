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
	void ResetCheckVector() { std::fill(m_check_vector.begin(), m_check_vector.end(), 0); };

private:
    std::vector<UINT> m_check_vector; //Layer ���� �浹 üũ ��Ʈ����

	std::map<ULONGLONG, bool> m_collision_check_map; //�浹ü ���� ���� ������ �浹 ���� 
};

