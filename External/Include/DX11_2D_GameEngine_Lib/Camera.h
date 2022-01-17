#pragma once
#include "IComponent.h"

class Camera : public IComponent
{
public:
    Camera();
    explicit Camera(const Camera& origin);
    ~Camera();

    void FinalUpdate() override;

public:
    void UpdateMatrix();

    //Rendering 시점 별로 그릴 오브젝트들을 분류하는 작업
    void SortObjects();

    void RenderForwardObjects();
    void RenderParticleObjects();
    void RenderPostEffectObjects();

protected:
    //뷰 행렬(메트릭스) 업데이트
    void UpdateViewMatrix();
    //투영 행렬(메트릭스) 업데이트
    void UpdateProjectionMatrix();

public:
    const Vector3 Picking();

private:
    const Vector3 ScreenToWorld(const Vector2& mouse_position);

public:
    const int& GetCameraIndex() { return m_camera_index; }
    void SetCameraIndex(const int& camera_index) { m_camera_index = camera_index; }

    void SetMainCamera() { m_camera_index = 0; }

    const ProjectionType& GetProjectionType() const { return m_projection_type; }
    void SetProjectionType(const ProjectionType& projection_type);

    //ProjectionType::Orthographic(직교 투영 전용)
    const Vector2& GetSize() const { return m_size; }
    void SetSize(const Vector2& size) { m_size = size; }

    const float& GetFov() const { return m_fov; }
    void SetFov(const float& fov) { m_fov = fov; }

    const float& GetNearZ() const { return m_near_z; }
    void SetNearZ(const float& near_z) { m_near_z = near_z; }

    const float& GetFarZ() const { return m_far_z; }
    void SetFarZ(const float& far_z) { m_far_z = far_z; }

    const Matrix& GetViewMatrix() const { return m_view_matrix; }
    const Matrix& GetProjectionMatrix() const { return m_projection_matrix; }

public:
     void CullingLayer(const UINT& layer_index); //토글 방식으로 구현(On/Off)
     void CullingEverything() { m_culling_layer = 0xffffffff; } //32비트를 모두 1로 채움
     void CullingNothing() { m_culling_layer = 0; } //32비트를 모두 0으로 채움

     const UINT& GetCullingLayer() { return m_culling_layer; }

public:
    void SaveToScene(FILE* p_file) override;
    void LoadFromScene(FILE* p_file) override;

public:
    CLONE(Camera);

protected:
    //RenderManager에 등록될 카메라 인덱스 값
    //메인 카메라의 index는 항상 0으로 고정
    int m_camera_index = -1; 

    //카메라 culling layer
    UINT m_culling_layer = 0;

    //카메라 투영 타입
    ProjectionType m_projection_type = ProjectionType::Orthographic;

    //ProjectionType::Orthographic(직교 투영 전용)
    //Size가 늘어남에 따라 한 화면에 보여지는 화면의 해상도 증가(커질수록 줌아웃, 작아질수록 줌인)
    Vector2 m_size = Vector2::One;
    
    //Field of View
    float m_fov = Math::ToRadian(47); //47은 표준 렌즈의 시야각 값
    float m_near_z = 0.3f;
    float m_far_z = 1000.0f;
   
    //View Space 특징
    //1. 카메라는 원점에 있음 (카메라가 이동했다면 이동한 만큼의 반대 방향으로 모든 오브젝트에 위치계산 적용)
    //2. 카메라가 바라보는 방향은 z축과 평행
    Matrix m_view_matrix = Matrix::Identity;
    Matrix m_projection_matrix = Matrix::Identity;

    std::vector<GameObject*> m_forward_object_vector;
    std::vector<GameObject*> m_particle_object_vector;
    std::vector<GameObject*> m_post_effect_object_vector;
};

