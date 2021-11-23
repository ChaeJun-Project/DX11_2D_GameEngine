#pragma once
#include "IComponent.h"

class Camera : public IComponent
{
public:
    Camera();
    explicit Camera(const Camera& origin);
    ~Camera();

    void Update() override;
    void FinalUpdate() override;

    //Rendering 시점 별로 그릴 오브젝트들을 분류하는 작업
    void SortObjects();

    void RenderForwardObjects();
    void RenderParticleObjects();
    void RenderPostEffectObjects();

private:
    //뷰 행렬(메트릭스) 업데이트
    void UpdateViewMatrix();
    //투영 행렬(메트릭스) 업데이트
    void UpdateProjectionMatrix();

public:
    void Picking();

private:
    const Vector3& ScreenToWorld(const Vector2& mouse_position);

public:
    void SetMainCamera() { this->m_camera_index = 0; }

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

public:
     void CullingLayer(UINT layer_index); //토글 방식으로 구현(On/Off)
     void CullingEverything() { this->m_culling_layer = 0xffffffff; } //32비트를 모두 1로 채움
     void CullingNothing() { this->m_culling_layer = 1 << 0; } //32비트를 모두 0으로 채움

public:
    CLONE(Camera);

private:
    //RenderManager에 등록될 카메라 인덱스 값
    //메인 카메라의 index는 항상 0으로 고정
    int m_camera_index = -1; 

    //카메라 투영 타입
    ProjectionType m_projection_type = ProjectionType::Orthographic;
    
    //Field of View
    float m_fov = Math::ToRadian(47); //47은 표준 렌즈의 시야각 값
    float m_near_z = 0.3f;
    float m_far_z = 1000.0f;
    float m_speed = 3.0f;

    Matrix m_view_matrix = Matrix::Identity;
    Matrix m_projection_matrix = Matrix::Identity;

    UINT m_culling_layer = 1 << 0;

    std::vector<GameObject*> m_forward_object_vector;
    std::vector<GameObject*> m_particle_object_vector;
    std::vector<GameObject*> m_post_effect_object_vector;
};

