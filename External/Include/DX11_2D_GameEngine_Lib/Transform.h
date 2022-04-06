#pragma once

#include "IComponent.h"

class RectTransform;

class Transform : public IComponent
{
public:
	Transform();
	Transform(RectTransform& rect_transform);
	~Transform() = default;

protected:
	Transform(const ComponentType& component_type);

public:
	void FinalUpdate() override;

public:
	//=====================================================================
	// [Constant Buffer]
	//=====================================================================
	void UpdateConstantBuffer();

private:
	//DirectX�� ��켱 ������� GPU���� �����ϱ� ������
	//World Matrix�� ����� �� Scale -> Rotation -> Translation (SRT) ������ ������ �ؼ� ����
	void UpdateWorldMatrix();

public:
	//=====================================================================
	// [Local]
	//=====================================================================
	
	//Translation
	const Vector3& GetLocalTranslation() const { return m_local_translation; }
	void SetLocalTranslation(const Vector3& local_translation);

	//Rotation
	const Quaternion& GetLocalRotation() const { return m_local_rotation; }
	void SetLocalRotation(const Quaternion& local_rotation);

	//Scale
	const Vector3& GetLocalScale() const { return m_local_scale; }
	void SetLocalScale(const Vector3& local_scale);

public:
	//=====================================================================
	// [World]
	//=====================================================================
	//Translation
	const Vector3 GetTranslation() { return m_origin_world_matrix.GetTranslation(); }
	void SetTranslation(const Vector3& translation);

	//Rotation
	const Quaternion GetRotation() { return m_origin_world_matrix.GetRotation(); }
	void SetRotation(const Quaternion& rotation);

	//Scale
	const Vector3 GetScale() { return m_origin_world_matrix.GetScale(); }
	void SetScale(const Vector3& scale);

	//Direction
	const Vector3 GetRightVector() const { return Vector3::Right * m_local_rotation; }
	const Vector3 GetUpVector() const { return Vector3::Up * m_local_rotation; }
	const Vector3 GetForwardVector() const { return Vector3::Forward * m_local_rotation; }

	//World Matrix
	const Matrix GetParentOriginWorldMatrix() const { return m_parent_origin_world_matrix; }
	const Matrix GetOriginWorldMatrix() const { return m_origin_world_matrix; }
	const Matrix GetWorldMatrix() const { return m_world_matrix; }
	
public:
	//�ڽ� GameObject���� ��Ӱ��谡 �����Ǿ��� ��� ȣ��
	virtual void InitialzieProperty();

public:
	//Mesh Scale
	void SetMeshScale(const UINT& width, const UINT& height) { m_mesh_scale = Vector3(static_cast<float>(width), static_cast<float>(height), 1.0f); }
	const Vector3& GetMeshScale() const { return m_mesh_scale; }

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(Transform);

protected:
	Vector3 m_local_translation = Vector3::Zero; //xyz ��� 0.0f�� �ʱ�ȭ
	Quaternion m_local_rotation = Quaternion::Identity;
	Vector3 m_local_scale = Vector3::One; //xyz ��� 1.0f�� �ʱ�ȭ

	Matrix m_parent_origin_world_matrix = Matrix::Identity;
	Matrix m_origin_world_matrix = Matrix::Identity; //m_mesh_scale�� ������ ���� ���� World Matrix => �ڽ� GameObject�� World Matrix ���꿡 �ʿ�
	Matrix m_world_matrix = Matrix::Identity;		 //m_mesh_scale�� ���� ���� ���Ǵ� World Matrix => ���� �������� ���Ǵ� World Matrix

private:
	Vector3 m_mesh_scale = Vector3::One;

};

