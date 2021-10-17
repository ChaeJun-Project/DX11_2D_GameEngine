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
    
    std::shared_ptr<Material> GetMaterial() { SAFE_GET_POINTER(this->m_p_material); }

private:
    std::shared_ptr<Mesh> m_p_mesh;
    std::shared_ptr<Material> m_p_material;
};

