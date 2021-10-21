#pragma once
#include "IComponent.h"

class Collider2D final : public IComponent
{
public:
	Collider2D();
	explicit Collider2D(const Collider2D& origin);
	~Collider2D();

	void FinalUpdate() override;
	void Render();

	//=====================================================================
	// [Constant Buffer]
	//=====================================================================
	void UpdateConstantBuffer();

private:
	void UpdateColliderWorldMatrix();

public:
    void OnCollisionEnter(std::shared_ptr<Collider2D>& other_collider);
	void OnCollisionExit(std::shared_ptr<Collider2D>& other_collider);
	void OnCollision(std::shared_ptr<Collider2D>& other_collider);

public:
    void SetOffsetPos(const Vector3& offset_position) { this->m_offset_position = offset_position; }
	void SetOffsetScale(const Vector3& offset_scale) { this->m_offset_scale = offset_scale; }

	const Matrix& GetColliderWorldMatrix() { return this->m_collider_world_matrix; }

private:
    Vector3 m_offset_position = Vector3::Zero;
	Vector3 m_offset_scale = Vector3::One; //����Ƽ������ Size

	Matrix m_collider_world_matrix = Matrix::Identity;

	UINT m_collision_count = 0;

	std::shared_ptr<Mesh> m_p_mesh;
	std::shared_ptr<Material> m_p_material;
};

