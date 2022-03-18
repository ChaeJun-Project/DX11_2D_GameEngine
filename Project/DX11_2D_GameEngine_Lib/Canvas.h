#pragma once

#include "IComponent.h"

//<summary>
//UI를 다루기 위한 컴포넌트
//</summary>

class Camera;

class Canvas final : public IComponent
{
public:
	Canvas();
	explicit Canvas(const Canvas& origin); //=> 복사 생성자 호출 시 얕은 복사 사용
	~Canvas() = default;

	void FinalUpdate() override;
	void Render();

	//=====================================================================
	// [Constant Buffer]
	//=====================================================================
	void UpdateConstantBuffer();

private:
	void UpdateCanvasWorldMatrix();

public:
    const Vector2 GetCanvasResolution() const { return m_canvas_resolution; }
	void SetCanvasResolution(const Vector2& canvas_resolution) { m_canvas_resolution = canvas_resolution; }

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(Canvas);

private:
	Camera* p_ui_camara = nullptr;

	Matrix m_canvas_world_matrix = Matrix::Identity;

	std::shared_ptr<Material> m_p_material;
	std::shared_ptr<Mesh> m_p_mesh;

    Vector2 m_canvas_resolution = Vector2::Zero;
};

