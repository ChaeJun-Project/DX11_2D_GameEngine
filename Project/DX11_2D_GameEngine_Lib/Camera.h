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

    //Rendering ���� ���� �׸� ������Ʈ���� �з��ϴ� �۾�
    void SortObjects();

    void RenderForwardObjects();
    void RenderParticleObjects();
    void RenderPostEffectObjects();

private:
    //�� ���(��Ʈ����) ������Ʈ
    void UpdateViewMatrix();
    //���� ���(��Ʈ����) ������Ʈ
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
     void CullingLayer(UINT layer_index); //��� ������� ����(On/Off)
     void CullingEverything() { this->m_culling_layer = 0xffffffff; } //32��Ʈ�� ��� 1�� ä��
     void CullingNothing() { this->m_culling_layer = 1 << 0; } //32��Ʈ�� ��� 0���� ä��

public:
    CLONE(Camera);

private:
    //RenderManager�� ��ϵ� ī�޶� �ε��� ��
    //���� ī�޶��� index�� �׻� 0���� ����
    int m_camera_index = -1; 

    //ī�޶� ���� Ÿ��
    ProjectionType m_projection_type = ProjectionType::Orthographic;
    
    //Field of View
    float m_fov = Math::ToRadian(47); //47�� ǥ�� ������ �þ߰� ��
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

