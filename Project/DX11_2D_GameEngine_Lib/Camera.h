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

    //Rendering ���� ���� �׸� ������Ʈ���� �з��ϴ� �۾�
    void SortObjects();

    void RenderForwardObjects();
    void RenderParticleObjects();
    void RenderPostEffectObjects();

protected:
    //�� ���(��Ʈ����) ������Ʈ
    void UpdateViewMatrix();
    //���� ���(��Ʈ����) ������Ʈ
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
     void CullingLayer(const UINT& layer_index); //��� ������� ����(On/Off)
     void CullingEverything() { m_culling_layer = 0xffffffff; } //32��Ʈ�� ��� 1�� ä��
     void CullingNothing() { m_culling_layer = 0; } //32��Ʈ�� ��� 0���� ä��

     const UINT& GetCullingLayer() { return m_culling_layer; }

public:
    void SaveToScene(FILE* p_file) override;
    void LoadFromScene(FILE* p_file) override;

public:
    CLONE(Camera);

protected:
    //RenderManager�� ��ϵ� ī�޶� �ε��� ��
    //���� ī�޶��� index�� �׻� 0���� ����
    int m_camera_index = -1; 

    //ī�޶� culling layer
    UINT m_culling_layer = 0;

    //ī�޶� ���� Ÿ��
    ProjectionType m_projection_type = ProjectionType::Orthographic;

    //ProjectionType::Orthographic(���� ���� ����)
    //Size�� �þ�� ���� �� ȭ�鿡 �������� ȭ���� �ػ� ����(Ŀ������ �ܾƿ�, �۾������� ����)
    Vector2 m_size = Vector2::One;
    
    //Field of View
    float m_fov = Math::ToRadian(47); //47�� ǥ�� ������ �þ߰� ��
    float m_near_z = 0.3f;
    float m_far_z = 1000.0f;
   
    //View Space Ư¡
    //1. ī�޶�� ������ ���� (ī�޶� �̵��ߴٸ� �̵��� ��ŭ�� �ݴ� �������� ��� ������Ʈ�� ��ġ��� ����)
    //2. ī�޶� �ٶ󺸴� ������ z��� ����
    Matrix m_view_matrix = Matrix::Identity;
    Matrix m_projection_matrix = Matrix::Identity;

    std::vector<GameObject*> m_forward_object_vector;
    std::vector<GameObject*> m_particle_object_vector;
    std::vector<GameObject*> m_post_effect_object_vector;
};

