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
    void SetMainCamera() { m_camera_index = 0; }

    const ProjectionType& GetProjectionType() const { return m_projection_type; }
    void SetProjectionType(const ProjectionType& projection_type) { m_projection_type = projection_type; }

    //ProjectionType::Orthographic(���� ���� ����)
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
     void CullingLayer(UINT layer_index); //��� ������� ����(On/Off)
     void CullingEverything() { m_culling_layer = 0xffffffff; } //32��Ʈ�� ��� 1�� ä��
     void CullingNothing() { m_culling_layer = 1 << 0; } //32��Ʈ�� ��� 0���� ä��

public:
    CLONE(Camera);

private:
    //RenderManager�� ��ϵ� ī�޶� �ε��� ��
    //���� ī�޶��� index�� �׻� 0���� ����
    int m_camera_index = -1; 

    //ī�޶� ���� Ÿ��
    ProjectionType m_projection_type = ProjectionType::Orthographic;

    //ProjectionType::Orthographic(���� ���� ����)
    //Size�� �þ�� ���� �� ȭ�鿡 �������� ȭ���� �ػ� ����(Ŀ������ �ܾƿ�, �۾������� ����)
    Vector2 m_size = Vector2::One;
    
    //Field of View
    float m_fov = Math::ToRadian(47); //47�� ǥ�� ������ �þ߰� ��
    float m_near_z = 0.3f;
    float m_far_z = 1000.0f;
    float m_speed = 100.0f;

    Matrix m_view_matrix = Matrix::Identity;
    Matrix m_projection_matrix = Matrix::Identity;

    UINT m_culling_layer = 1 << 0;

    std::vector<GameObject*> m_forward_object_vector;
    std::vector<GameObject*> m_particle_object_vector;
    std::vector<GameObject*> m_post_effect_object_vector;
};

