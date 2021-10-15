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

	//DirectX�� ��켱 ������� GPU���� �����ϱ� ������
	//World Matrix�� ����� �� Scale -> Rotation -> Translation (SRT) ������ ������ �ؼ� ����
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
	Vector3 m_local_translation = Vector3::Zero; //xyz ��� 0.0f�� �ʱ�ȭ
	Quaternion m_local_rotation = Quaternion::Identity;
	Vector3 m_local_scale = Vector3::One; //xyz ��� 1.0f�� �ʱ�ȭ

	Matrix m_world_matrix = Matrix::Identity;
};

