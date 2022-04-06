#pragma once

#include "Transform.h"

class RectTransform final : public Transform
{
public:
	RectTransform();
	RectTransform(Transform& transform);
	~RectTransform() = default;

	void FinalUpdate();

public:
	void operator=(const Transform& transform);

public:
	//=====================================================================
	// [Constant Buffer]
	//=====================================================================
	void UpdateConstantBuffer();

private:
	void UpdateWorldMatrix();

private:
	void UpdateAnchorPosition();
	void UpdateAnchorMatrix(const Vector2& widget_size);

public:
	//Widget Size
	void SetWidgetSize(const Vector2& widget_size) { m_widget_size = widget_size; }
	const Vector2 GetWidgetSize() const { return m_widget_size; }

	//Anchor
	void SetAnchor(const Vector2& anchor) { m_anchor = anchor; }
	const Vector2 GetAnchor() const { return m_anchor; }

	//Anchor Matrix
	const Matrix GetAnchorMatrix() const { return m_anchor_matrix; }

public:
	void InitialzieProperty() override;

private:
	void SaveToScene(FILE* p_file);
	void LoadFromScene(FILE* p_file);

private:
	CLONE(RectTransform);

private:
	Vector2 m_widget_size = Vector2(100.0f, 100.0f);

	Vector2 m_anchor = Vector2(0.5f, 0.5f);
	Vector2 m_pre_anchor = m_anchor;
	Matrix m_anchor_matrix = Matrix::Identity;
};

