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

public:
	//Widget Size
	void SetWidgetSize(const Vector2& widget_size) { m_widget_size = widget_size; }
	const Vector2& GetWidgetSize() const { return m_widget_size; }

private:
	void SaveToScene(FILE* p_file);
	void LoadFromScene(FILE* p_file);

private:
	CLONE(RectTransform);

private:
	Vector2 m_widget_size = Vector2(100.0f, 100.0f);

	//TODO: Anchor 생각해보기
};

