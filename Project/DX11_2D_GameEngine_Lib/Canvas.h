#pragma once

#include "IComponent.h"

//<summary>
//UI�� �ٷ�� ���� ������Ʈ
//</summary>

class GameObject;
class RectTransform;

class Canvas final : public IComponent
{
public:
	Canvas();
	explicit Canvas(const Canvas& origin); //=> ���� ������ ȣ�� �� ���� ���� ���
	~Canvas();

	void Start() override;
	void FinalUpdate() override;
	void Render();

	//=====================================================================
	// [Constant Buffer]
	//=====================================================================
	void UpdateConstantBuffer();

private:
	void UpdateCanvasWorldMatrix();

public:
	std::string GetUICameraObjectName() const { return m_ui_camera_object_name; }
	void SetUICameraObjectName(const std::string& ui_camera_object_name) { m_ui_camera_object_name = ui_camera_object_name; }

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(Canvas);

private:
	std::string m_ui_camera_object_name;
	GameObject* m_p_ui_camera_object = nullptr;

	Matrix m_canvas_world_matrix = Matrix::Identity;

	std::shared_ptr<Material> m_p_material;
	std::shared_ptr<Mesh> m_p_mesh;

	RectTransform* m_p_rect_transform = nullptr;
};

