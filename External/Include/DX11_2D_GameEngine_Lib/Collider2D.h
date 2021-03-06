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
	void ChangeColliderBoxColorGreen();
	void ChangeColliderBoxColorRed();

public:
    void OnCollisionEnter(Collider2D* other_collider);
	void OnCollisionStay(Collider2D* other_collider);
	void OnCollisionExit(Collider2D* other_collider);

public:
    //Offset Position
    Vector2 GetOffsetPosition() const { return m_offset_position; }
    void SetOffsetPosition(const Vector2& offset_position) { m_offset_position = offset_position; }

	//Offset Scale
	Vector2 GetOffsetScale() const { return m_offset_scale; }
	void SetOffsetScale(const Vector2& offset_scale) { m_offset_scale = offset_scale; }

	Vector3 GetDefaultSize() { return Vector3(m_default_size.x, m_default_size.y, 1.0f); }
	const Matrix& GetColliderWorldMatrix() { return m_collider2D_world_matrix; }

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(Collider2D);

private:
	Vector2 m_default_size = Vector2(100.0f, 100.f);

    Vector2 m_offset_position = Vector2::Zero;
	Vector2 m_offset_scale = Vector2::One; //유니티에서는 Size

	Matrix m_collider2D_world_matrix = Matrix::Identity;

	UINT m_collision_count = 0;

	std::shared_ptr<Material> m_p_material;
	std::shared_ptr<Mesh> m_p_mesh;
};

