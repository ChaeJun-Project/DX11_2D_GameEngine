#pragma once
#include "IComponent.h"

enum class ImageType : UINT
{
   Default = 0,
   Filed = 1,
};

enum class FillMethodType : UINT
{
    Horizontal = 1,
    Vertical = 2,
};

//수평 채우기 방향
enum class HorizontalDirectionType : UINT
{
     Left = 1,
     Right = 2
};

//수직 채우기 방향
enum class VerticalDirectionType : UINT
{
     Bottom = 1,
     Top = 2
};

class ImageRenderer final : public IComponent
{
public:
    ImageRenderer();
    explicit ImageRenderer(const ImageRenderer& origin);
    ~ImageRenderer();

    void FinalUpdate() override {};

    void Render();

private:
    void SetImageRendererInfo();

public:
    std::shared_ptr<Texture> GetTexture() { SAFE_GET_POINTER(m_p_texture); }
    void SetTexture(const std::shared_ptr<Texture>& p_texture);

    const Vector4& GetSpriteTextureColor() { return m_texture_color; }
    void SetSpriteTextureColor(const Vector4& texture_color) { m_texture_color = texture_color; }

    std::shared_ptr<Material> GetMaterial() { SAFE_GET_POINTER(m_p_material); }
    void SetMaterial(const std::shared_ptr<Material>& p_material);

    std::shared_ptr<Mesh> GetMesh() { SAFE_GET_POINTER(m_p_mesh); }
    void SetMesh(const std::shared_ptr<Mesh>& p_mesh);

private:
    void SaveToScene(FILE* p_file) override;
    void LoadFromScene(FILE* p_file) override;

private:
    CLONE(ImageRenderer);

private:
    std::shared_ptr<Texture> m_p_texture = nullptr;
    Vector4 m_texture_color = Vector4::White;

    std::shared_ptr<Material> m_p_material = nullptr;
    std::shared_ptr<Mesh> m_p_mesh = nullptr;

    ImageType m_image_type = ImageType::Default;
    FillMethodType m_fill_method_type = FillMethodType::Horizontal;
    HorizontalDirectionType m_horizontal_direction_type = HorizontalDirectionType::Left;
    VerticalDirectionType m_vertical_direction_type = VerticalDirectionType::Bottom;
    float m_fill_amount = 1.0f;
};

