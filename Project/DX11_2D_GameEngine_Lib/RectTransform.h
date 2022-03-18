#pragma once

#include "IComponent.h"

class Transform;

class RectTransform final : public IComponent
{
public:
	RectTransform();
	~RectTransform() = default;

	void FinalUpdate() override;

public:
	void operator=(const Transform& transform);

public:
	//=====================================================================
	// [Constant Buffer]
	//=====================================================================
	void UpdateConstantBuffer();

private:
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
	const Vector3 GetTranslation() { return m_world_matrix.GetTranslation(); }
	void SetTranslation(const Vector3& translation);

	//Rotation
	const Quaternion GetRotation() { return m_world_matrix.GetRotation(); }
	void SetRotation(const Quaternion& rotation);

	//Scale
	const Vector3 GetScale() { return m_world_matrix.GetScale(); }
	void SetScale(const Vector3& scale);

	//World Matrix
	const Matrix& GetParentOriginWorldMatrix() const { return m_parent_origin_world_matrix; }
	const Matrix& GetOriginWorldMatrix() const { return m_origin_world_matrix; }
	const Matrix& GetWorldMatrix() const { return m_world_matrix; }

public:
	//Widget Size
	void SetWidgetSize(const Vector2& widget_size) { m_widget_size = widget_size; }
	const Vector2& GetWidgetSize() const { return m_widget_size; }

public:
	//�ڽ� GameObject���� ��Ӱ��谡 �����Ǿ��� ��� ȣ��
	void InitialzieProperty();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(RectTransform);

private:
	Vector3 m_local_translation = Vector3::Zero; //xyz ��� 0.0f�� �ʱ�ȭ
	Vector2 m_widget_size = Vector2(100.0f, 100.0f);

	//TODO: Anchor �����غ���

	Quaternion m_local_rotation = Quaternion::Identity;
	Vector3 m_local_scale = Vector3::One; //xyz ��� 1.0f�� �ʱ�ȭ

	Matrix m_parent_origin_world_matrix = Matrix::Identity;
	Matrix m_origin_world_matrix = Matrix::Identity; //m_widget_size�� ������ ���� ���� World Matrix => �ڽ� GameObject�� World Matrix ���꿡 �ʿ�
	Matrix m_world_matrix = Matrix::Identity;		 //m_widget_size�� ���� ���� ���Ǵ� World Matrix => ���� �������� ���Ǵ� World Matrix
};

