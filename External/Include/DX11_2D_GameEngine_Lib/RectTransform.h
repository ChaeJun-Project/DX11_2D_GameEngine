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
	//자식 GameObject에서 상속관계가 해제되었을 경우 호출
	void InitialzieProperty();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(RectTransform);

private:
	Vector3 m_local_translation = Vector3::Zero; //xyz 모두 0.0f로 초기화
	Vector2 m_widget_size = Vector2(100.0f, 100.0f);

	//TODO: Anchor 생각해보기

	Quaternion m_local_rotation = Quaternion::Identity;
	Vector3 m_local_scale = Vector3::One; //xyz 모두 1.0f로 초기화

	Matrix m_parent_origin_world_matrix = Matrix::Identity;
	Matrix m_origin_world_matrix = Matrix::Identity; //m_widget_size을 곱하지 않은 원본 World Matrix => 자식 GameObject의 World Matrix 연산에 필요
	Matrix m_world_matrix = Matrix::Identity;		 //m_widget_size을 곱한 실제 사용되는 World Matrix => 실제 렌더링에 사용되는 World Matrix
};

