#pragma once
#include "IComponent.h"

class Texture;
class Mesh;
class Material;

class SpriteRenderer : public IComponent
{
public:
    SpriteRenderer();
    explicit SpriteRenderer(const SpriteRenderer& origin);
    ~SpriteRenderer();

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
    CLONE(SpriteRenderer);

private:
    std::shared_ptr<Texture> m_p_sprite = nullptr;

    std::shared_ptr<Mesh> m_p_mesh = nullptr;
  
    std::shared_ptr<Material> m_p_current_material = nullptr;//현재 사용할 Material
    std::shared_ptr<Material> m_p_shared_material= nullptr; //공유 Material
    std::shared_ptr<Material> m_p_cloned_material= nullptr; //공유 Material 복사본

    std::shared_ptr<Material> m_p_border = nullptr; //오브젝트 테두리
};

