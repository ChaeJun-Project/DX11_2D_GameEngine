#pragma once
#include "IComponent.h"

class Camera : public IComponent
{
public:
    Camera(GameObject* p_game_object);
    ~Camera() = default;

    void Update() override;
    void FinalUpdate() override;

private:
    //뷰 행렬(메트릭스) 업데이트
    void UpdateViewMatrix();
    //투영 행렬(메트릭스) 업데이트
    void UpdateProjectionMatrix();

public:
    const ProjectionType& GetProjectionType() const { return this->m_projection_type; }
    void SetProjectionType(const ProjectionType& projection_type) { this->m_projection_type = projection_type; }

    const float& GetFov() const { return this->m_fov; }
    void SetFov(const float& fov) { this->m_fov = fov; }

    const float& GetNearZ() const { return this->m_near_z; }
    void SetNearZ(const float& near_z) { this->m_near_z = m_near_z; }

    const float& GetFarZ() const { return this->m_far_z; }
    void SetFarZ(const float& far_z) { this->m_far_z = m_far_z; }

    const Matrix& GetViewMatrix() const { return this->m_view_matrix; }
    const Matrix& GetProjectionMatrix() const { return this->m_projection_matrix; }

private:
    //카메라 투영 타입
    ProjectionType m_projection_type = ProjectionType::Orthographic;
    
    //Field of View
    float m_fov = Math::ToRadian(47); //47은 표준 렌즈의 시야각 값
    float m_near_z = 0.3f;
    float m_far_z = 1000.0f;

    Matrix m_view_matrix = Matrix::Identity;
    Matrix m_projection_matrix = Matrix::Identity;
};

