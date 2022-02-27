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

    void FinalUpdate() override {};

    void Render();

public:
    std::shared_ptr<Texture> GetSpriteTexture() { SAFE_GET_POINTER(m_p_sprite_texture); }
    void SetSpriteTexture(const std::shared_ptr<Texture>& p_sprite_texture) { m_p_sprite_texture = p_sprite_texture; }

    const Vector4& GetSpriteTextureColor() { return m_sprite_texture_color; }
    void SetSpriteTextureColor(const Vector4& sprite_texture_color) { m_sprite_texture_color = sprite_texture_color; }

    std::shared_ptr<Mesh> GetMesh() { SAFE_GET_POINTER(m_p_mesh); }
    void SetMesh(const std::shared_ptr<Mesh>& p_mesh) { m_p_mesh= p_mesh; }

    void SetMaterial(const std::shared_ptr<Material>& p_material);
    std::shared_ptr<Material> GetMaterial() { SAFE_GET_POINTER(m_p_material); }

private:
    void SaveToScene(FILE* p_file) override;
    void LoadFromScene(FILE* p_file) override;

private:
    CLONE(SpriteRenderer);

private:
    std::shared_ptr<Texture> m_p_sprite_texture = nullptr;
    Vector4 m_sprite_texture_color = Vector4::White;

    std::shared_ptr<Material> m_p_material = nullptr;
    std::shared_ptr<Mesh> m_p_mesh = nullptr;
};

