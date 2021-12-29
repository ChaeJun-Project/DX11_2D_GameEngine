#pragma once
#include "IComponent.h"

class Texture;
class Mesh;
class Material;

class SpriteRenderer : public IComponent
{
public:
    SpriteRenderer();
    ~SpriteRenderer();

    void FinalUpdate() override {};

    void Render();

public:
    std::shared_ptr<Texture> GetSpriteTexture() { SAFE_GET_POINTER(m_p_sprite_texture); }
    void SetSpriteTexture(const std::shared_ptr<Texture>& p_sprite_texture) { m_p_sprite_texture = p_sprite_texture; }

    std::shared_ptr<Mesh> GetMesh() { SAFE_GET_POINTER(m_p_mesh); }
    void SetMesh(const std::shared_ptr<Mesh>& p_mesh) { m_p_mesh= p_mesh; }

    void SetMaterial(const std::shared_ptr<Material>& p_material);
    std::shared_ptr<Material> GetMaterial() { SAFE_GET_POINTER(m_p_material); }

public:
    virtual void SaveToScene(FILE* p_file);
    virtual void LoadFromScene(FILE* p_file);

public:
    CLONE(SpriteRenderer);

private:
    std::shared_ptr<Texture> m_p_sprite_texture = nullptr;

    std::shared_ptr<Mesh> m_p_mesh = nullptr;
    std::shared_ptr<Material> m_p_material = nullptr;
};

