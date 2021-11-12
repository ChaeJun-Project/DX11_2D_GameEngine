#pragma once
#include "IComponent.h"

class Mesh;
class Material;

class Renderer : public IComponent
{
public:
    Renderer();
    explicit Renderer(const Renderer& origin);
    ~Renderer();

    void FinalUpdate() override;

    void Render();

public:
    std::shared_ptr<Mesh> GetMesh() { SAFE_GET_POINTER(this->m_p_mesh); }
    void SetMesh(const std::shared_ptr<Mesh>& p_mesh) { this->m_p_mesh= p_mesh; }

    void SetMaterial(const std::shared_ptr<Material>& p_current_material);
    std::shared_ptr<Material> GetMaterial() { SAFE_GET_POINTER(this->m_p_current_material); }
    std::shared_ptr<Material> GetSharedMaterial();
    std::shared_ptr<Material> GetClonedMaterial();

public:
    CLONE(Renderer);

private:
    std::shared_ptr<Mesh> m_p_mesh = nullptr;
    
    UINT texture_width = 0;
    UINT texture_height = 0;

    std::shared_ptr<Material> m_p_current_material = nullptr;//현재 사용할 Material
    std::shared_ptr<Material> m_p_shared_material= nullptr; //공유 Material
    std::shared_ptr<Material> m_p_cloned_material= nullptr; //공유 Material 복사본

    std::shared_ptr<Material> m_p_border = nullptr; //오브젝트 테두리
};

