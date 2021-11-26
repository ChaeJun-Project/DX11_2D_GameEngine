#pragma once

#include "IComponent.h"

class Transform final : public IComponent
{
public:
	Transform();
	explicit Transform(const Transform& origin) = default; //=> 복사 생성자 호출 시 얕은 복사 사용
	~Transform() = default;

	void FinalUpdate() override;

	void Translate(const Vector3& move);

	//=====================================================================
	// [Constant Buffer]
	//=====================================================================
	void UpdateConstantBuffer();

private:
	//=====================================================================
	// [Local]
	//=====================================================================
	//Translation
	void SetLocalTranslation(const Vector3& local_translation);

	//Rotation
	void SetLocalRotation(const Quaternion& local_rotation);

	//Scale
	void SetLocalScale(const Vector3& local_scale);

	//DirectX는 행우선 방식으로 GPU에서 연산하기 때문에
	//World Matrix를 계산할 때 Scale -> Rotation -> Translation (SRT) 순서로 연산을 해서 구함
	void UpdateWorldMatrix();

public:
	//=====================================================================
	// [Local]
	//=====================================================================
	//Translation
	const Vector3& GetLocalTranslation() const { return m_local_translation; }

	//Rotation
	const Quaternion& GetLocalRotation() const { return m_local_rotation; }

	//Scale
	const Vector3& GetLocalScale() const { return m_local_scale; }

	//=====================================================================
	// [World]
	//=====================================================================
	//Translation
	const Vector3 GetTranslation() { return m_world_matrix.GetTranslation(); }
	void SetTranslation(const Vector3& translation);

	//Rotation
	const Quaternion GetRotation() { return m_world_matrix.GetRotation(); }
	void SetRotation(const Quaternion& rotation);

	//Scale
	const Vector3 GetScale() { return m_world_matrix.GetScale(); }
	void SetScale(const Vector3& scale);

	//Direction
	const Vector3 GetRightVector() const { return Vector3::Right * m_local_rotation; }
	const Vector3 GetUpVector() const { return Vector3::Up * m_local_rotation; }
	const Vector3 GetForwardVector() const { return Vector3::Forward * m_local_rotation; }

	//World Matrix
	const Matrix& GetWorldMatrix() const { return m_world_matrix; }
	const Vector3& GetWorldScale();

public:
	//Object Side
	const GameObjectSideState& GetObjectSideState() const { return m_game_object_side_state; }
	void SetObjectSideState(const GameObjectSideState& object_side_state) { m_game_object_side_state = object_side_state; }

	//Mesh Scale
	void SetMeshScale(const UINT& width, const UINT& height) { m_mesh_scale = Vector3(static_cast<float>(width), static_cast<float>(height), 1.0f); }
	const Vector3& GetMeshScale() const { return m_mesh_scale; }

public:
	CLONE(Transform);

private:
    Vector3 m_mesh_scale = Vector3::One;

	Vector3 m_local_translation = Vector3::Zero; //xyz 모두 0.0f로 초기화
	Quaternion m_local_rotation = Quaternion::Identity;
	Vector3 m_local_scale = Vector3::One; //xyz 모두 1.0f로 초기화

	Matrix m_world_matrix = Matrix::Identity;

	//Game Object Side
	GameObjectSideState m_game_object_side_state = GameObjectSideState::Right;
};

