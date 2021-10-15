#pragma once

#include "IComponent.h"

class Transform final : public IComponent
{
public:
	Transform();
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
	const Vector3& GetLocalTranslation() const { return this->m_local_translation; }

	//Rotation
	const Quaternion& GetLocalRotation() const { return this->m_local_rotation; }

	//Scale
	const Vector3& GetLocalScale() const { return this->m_local_scale; }

	//=====================================================================
	// [World]
	//=====================================================================
	//Translation
	const Vector3 GetTranslation() { return this->m_world_matrix.GetTranslation(); }
	void SetTranslation(const Vector3& translation);

	//Rotation
	const Quaternion GetRotation() { return this->m_world_matrix.GetRotation(); }
	void SetRotation(const Quaternion& rotation);

	//Scale
	const Vector3 GetScale() { return this->m_world_matrix.GetScale(); }
	void SetScale(const Vector3& scale);

	//Direction
	const Vector3 GetRightVector() const { return Vector3::Right * this->m_local_rotation; }
	const Vector3 GetUpVector() const { return Vector3::Up * this->m_local_rotation; }
	const Vector3 GetForwardVector() const { return Vector3::Forward * this->m_local_rotation; }

	//World Matrix
	const Matrix& GetWorldMatrix() const { return this->m_world_matrix; }

public:
    CLONE(Transform);

private:
	Vector3 m_local_translation = Vector3::Zero; //xyz 모두 0.0f로 초기화
	Quaternion m_local_rotation = Quaternion::Identity;
	Vector3 m_local_scale = Vector3::One; //xyz 모두 1.0f로 초기화

	Matrix m_world_matrix = Matrix::Identity;
};

